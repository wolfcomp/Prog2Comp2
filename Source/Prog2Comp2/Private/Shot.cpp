// Fill out your copyright notice in the Description page of Project Settings.


#include "Shot.h"

#include "Components/CapsuleComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/SpotLightComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "PlayerPawn.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/WeakObjectPtr.h"

float AShot::ExplosionIntensity() const
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
    // ease out quint and invert time
    delta = 1.0f - sqrt(1 - pow(1 - delta, 5));
    delta *= 20000.0f;
    return FMath::Clamp(delta, 0.0f, 20000.0f);
}

// Sets default values
AShot::AShot()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

	const ConstructorHelpers::FObjectFinder<USoundWave> soundWaveFinder(TEXT("SoundWave'/Game/MusicSFX/LaserShot.LaserShot'"));

	const ConstructorHelpers::FObjectFinder<UNiagaraSystem> shotEffectFinder(TEXT("NiagaraSystem'/Game/Blueprints/ShotEffect.ShotEffect'"));
	ShotEffect = shotEffectFinder.Object;

    const ConstructorHelpers::FObjectFinder<UNiagaraSystem> explosionEffectFinder(TEXT("NiagaraSystem'/Game/Blueprints/ShotHitEffect.ShotHitEffect'"));
    ExplosionEffect = explosionEffectFinder.Object;

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

	//Constructing sound
	AlienDeathSound = soundWaveFinder.Object;
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
    FScriptDelegate collisionBeginOverlap;
    collisionBeginOverlap.BindUFunction(this, "OnOverlapBegin");
    CollisionComponent->OnComponentBeginOverlap.Add(collisionBeginOverlap);
    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AShot::OnOverlapBegin);
}

// Called every frame
void AShot::Tick(float delta_time)
{
    Super::Tick(delta_time);
    Time += delta_time;
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
        Light2->SetIntensity(ExplosionIntensity());
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

void AShot::OnOverlapBegin(UPrimitiveComponent *overlapped_component, AActor *other_actor, UPrimitiveComponent *other_comp, int32 other_body_index, bool b_from_sweep, const FHitResult &sweep_result) { this->Explode(other_actor); }

void AShot::Explode(AActor *other_actor)
{
    if (!other_actor->Tags.FindByKey("Enemy"))
        return;

    CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
    ShotEffectComponent->Deactivate();
    Light1->SetIntensity(0.0f);
    if (!exploded)
    {
        GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Hit"));
        exploded = !exploded;
        ExplosionEffectComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionEffect, other_actor->GetActorLocation(), GetActorRotation());
        ExplosionEffectComponent->AttachToComponent(Light2, FAttachmentTransformRules::KeepRelativeTransform);

        if (const APlayerController *pc = GetWorld()->GetFirstPlayerController())
        {
            if (APlayerPawn *player = Cast<APlayerPawn>(pc->GetPawn()))
            {
                player->AddScore();
            }
        }
    }
    other_actor->Destroy();
    Time = LifeTime;
    Light2->SetIntensity(ExplosionIntensity());
}
