// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerHud.h"
#include "Shot.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/RectLightComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Components/RectLightComponent.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	const ConstructorHelpers::FObjectFinder<UStaticMesh> meshFinder(TEXT("StaticMesh'/Game/Models/SpaceShip/SpaceShip.SpaceShip'"));
	const ConstructorHelpers::FObjectFinder<UMaterial> meshMaterialFinder(TEXT("Material'/Game/Models/SpaceShip/MM_SpaceShip.MM_SpaceShip'"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root"));
	SetRootComponent(Mesh);
	Mesh->SetStaticMesh(meshFinder.Object);
	Mesh->SetMaterial(0, meshMaterialFinder.Object);
	Mesh->SetCollisionProfileName(TEXT("Custom"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetCollisionObjectType(ECC_Pawn);
	Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Mesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Mesh);
	SpringArm->bDoCollisionTest = false;
	SpringArm->TargetArmLength = 2000.f;
	SpringArm->SetRelativeRotation(FRotator(-30, 90, 0));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->bUsePawnControlRotation = false;
	Camera->SetupAttachment(SpringArm);

	RectLight = CreateDefaultSubobject<URectLightComponent>(TEXT("RectLight"));
    RectLight->SetupAttachment(Mesh);
    RectLight->SetRelativeLocation(FVector(0.f, -90.f, 250.f));
    RectLight->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	RectLight->SetSourceHeight(400);
    RectLight->SetSourceWidth(300);
	RectLight->SetIntensity(10000);

	const ConstructorHelpers::FObjectFinder<UInputAction> moveActionFinder(TEXT("InputAction'/Game/Input/Move.Move'"));
	MoveAction = moveActionFinder.Object;

	const ConstructorHelpers::FObjectFinder<UInputAction> lookActionFinder(TEXT("InputAction'/Game/Input/Look.Look'"));
	LookAction = lookActionFinder.Object;

	const ConstructorHelpers::FObjectFinder<UInputAction> shootActionFinder(TEXT("InputAction'/Game/Input/Shoot.Shoot'"));
	ShootAction = shootActionFinder.Object;

	static ConstructorHelpers::FObjectFinder<UBlueprint> shotFinder(TEXT("Blueprint'/Game/Blueprints/BP_Shot.BP_Shot'"));
	ShotClass = shotFinder.Object->GeneratedClass;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	ShotIndex = FMath::Rand();
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	if (PlayerHudTemplate)
	{
		PlayerHudWidget = CreateWidget<UPlayerHud>(GetWorld(), PlayerHudTemplate, FName("PlayerHud"));
	}
	if (PlayerHudWidget)
	{
		PlayerHudWidget->MyPlayer = this;
		PlayerHudWidget->AddToViewport();
	}
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	if (ReloadTimer >= ReloadTime)
	{
		Ammo = 20;
	}
	else
	{
		ReloadTimer += DeltaTime;
	}
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* inputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		inputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerPawn::Move);
		inputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &APlayerPawn::Shoot);
		inputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerPawn::Look);
	}
}

void APlayerPawn::Look(const FInputActionValue& Value)
{
	const float axisValue = Value.Get<float>();
	FRotator rotation = GetActorRotation();
	rotation.Yaw += axisValue * LookSpeed;
	SetActorRotation(rotation);
}


void APlayerPawn::Move(const FInputActionValue& Value)
{
	const FVector2D movementVector = Value.Get<FVector2D>();

	AddActorWorldOffset(GetActorRotation().RotateVector(FVector(-movementVector.X * Speed, movementVector.Y * Speed, 0.f)));
}

void APlayerPawn::Shoot(const FInputActionValue& Value)
{
	if (Ammo == 0)
		return;
	Ammo--;
	ReloadTimer = 0.f;
	FVector location = GetActorLocation();
	FRotator rotation = GetActorRotation();
	const FVector offset = ShotOffsets[ShotIndex++ % 4];
	location = location + rotation.RotateVector(offset);
	rotation = rotation + FRotator(0, -90, 0);
	GetWorld()->SpawnActor(ShotClass, &location, &rotation, FActorSpawnParameters());
	PlayShootSound();

}

void APlayerPawn::Collide(AActor* other_actor)
{
	if (other_actor->Tags.FindByKey("Enemy"))
		CurrentHealth -= 10;

	if (CurrentHealth <= 0)
	{
		this->Destroy();
		//show game over display
	}
}

void APlayerPawn::AddScore()
{
	Score++; }

void APlayerPawn::PlayShootSound()
{
    UGameplayStatics::PlaySound2D(GetWorld(), ShootSound, 1, 0, 0);
}
