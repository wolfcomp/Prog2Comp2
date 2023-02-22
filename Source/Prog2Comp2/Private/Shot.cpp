// Fill out your copyright notice in the Description page of Project Settings.


#include "Shot.h"

#include "Components/CapsuleComponent.h"
#include "NiagaraComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/PointLightComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "UObject/WeakObjectPtr.h"

float AShot::explosionIntensity() const
{
	float delta = Time - LifeTime;
	if (ExplosionDuration < 1.0f)
	{
		const float scale = 1.0f / ExplosionDuration;
		delta *= scale;
	}
	else
	{
		delta = delta / ExplosionDuration;
	}
	//ease out quint and invert time
	delta = 1.0f - sqrt(1 - pow(1 - delta, 5));
	delta *= 20000.0f;
	return FMath::Clamp(delta, 0.0f, 20000.0f);
}

// Sets default values
AShot::AShot()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	const ConstructorHelpers::FObjectFinder<UNiagaraSystem> ShotEffectFinder(TEXT("NiagaraSystem'/Game/Blueprints/ShotEffect.ShotEffect'"));
	ShotEffect = ShotEffectFinder.Object;

	const ConstructorHelpers::FObjectFinder<UNiagaraSystem> ExplosionEffectFinder(TEXT("NiagaraSystem'/Game/Blueprints/ShotHitEffect.ShotHitEffect'"));
	ExplosionEffect = ExplosionEffectFinder.Object;

	Light2 = CreateDefaultSubobject<UPointLightComponent>(TEXT("ExplosionLight"));
	SetRootComponent(Light2);
	Light2->SetLightColor(FLinearColor(0.313989f, 0.64448f, 1.0f, 1.0f));
	Light2->SetIntensity(0.0f);

#pragma region CollisionComponent
	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));
	CollisionComponent->AttachToComponent(Light2, FAttachmentTransformRules::KeepRelativeTransform);
	CollisionComponent->SetRelativeScale3D(FVector(0.5f, 0.5f, 2.25f));
	CollisionComponent->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));
	CollisionComponent->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
#pragma endregion

	Light1 = CreateDefaultSubobject<USpotLightComponent>(TEXT("ShotLight"));
	Light1->AttachToComponent(Light2, FAttachmentTransformRules::KeepRelativeTransform);
	Light1->SetRelativeLocation(FVector(-20.0f, 0.0f, 0.0f));
	Light1->SetLightColor(FLinearColor(0.313989f, 0.64448f, 1.0f, 1.0f));
	Light1->SetOuterConeAngle(20.0f);
	Light1->SetIntensity(20000.0f);
}

// Called when the game starts or when spawned
void AShot::BeginPlay()
{
	Super::BeginPlay();
	ShotEffectComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ShotEffect, GetActorLocation(), GetActorRotation());
	ShotEffectComponent->AttachToComponent(Light2, FAttachmentTransformRules::KeepRelativeTransform);
	CollisionComponent->SetCollisionProfileName(TEXT("Custom"));
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionObjectType(ECC_Pawn);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	CollisionComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	FScriptDelegate CollisionBeginOverlap;
	CollisionBeginOverlap.BindUFunction(this, "OnOverlapBegin");
	CollisionComponent->OnComponentBeginOverlap.Add(CollisionBeginOverlap);
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AShot::OnOverlapBegin);
}

// Called every frame
void AShot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Time += DeltaTime;
	if (Time >= LifeTime)
	{
		Light1->SetIntensity(0.0f);
		CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
		if (!exploded)
		{
			exploded = !exploded;
			ExplosionEffectComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionEffect, GetActorLocation(), GetActorRotation());
			ExplosionEffectComponent->AttachToComponent(Light2, FAttachmentTransformRules::KeepRelativeTransform);
		}
		Light2->SetIntensity(explosionIntensity());
	}
	else
	{
		FVector move = GetActorForwardVector();
		move *= -1;
		move *= Speed;
		AddActorWorldOffset(move);
		ShotEffectComponent->AddRelativeLocation(move);
	}
	if (Time >= LifeTime + FadeDuration)
	{
		Destroy();
	}
}

void AShot::OnOverlapBegin(UPrimitiveComponent* overlapped_component, AActor* other_actor,
	UPrimitiveComponent* other_comp, int32 other_body_index, bool b_from_sweep, const FHitResult& sweep_result)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Shot hit something"));
	this->Explode(other_actor);
}

void AShot::Explode(AActor* other_actor)
{
	if(!other_actor->Tags.FindByKey("Enemy"))
		return;
	//print other actor name to screen
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, other_actor->GetName());

	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	ShotEffectComponent->Deactivate();
	Light1->SetIntensity(0.0f);
	if (!exploded)
	{
		exploded = !exploded;
		ExplosionEffectComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionEffect, other_actor->GetActorLocation(), GetActorRotation());
		ExplosionEffectComponent->AttachToComponent(Light2, FAttachmentTransformRules::KeepRelativeTransform);
	}
	other_actor->Destroy();
	Time = LifeTime;
	Light2->SetIntensity(explosionIntensity());
}
