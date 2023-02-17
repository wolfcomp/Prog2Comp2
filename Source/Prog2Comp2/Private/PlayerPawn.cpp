// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Shot.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshFinder(TEXT("StaticMesh'/Game/Models/SpaceShip/SpaceShip.SpaceShip'"));
	const ConstructorHelpers::FObjectFinder<UMaterial> MeshMaterialFinder(TEXT("Material'/Game/Models/SpaceShip/MM_SpaceShip.MM_SpaceShip'"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root"));
	SetRootComponent(Mesh);
	Mesh->SetStaticMesh(MeshFinder.Object);
	Mesh->SetMaterial(0, MeshMaterialFinder.Object);
	Mesh->SetCollisionProfileName(TEXT("Custom"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetCollisionObjectType(ECC_Pawn);
	Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Mesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Mesh);
	SpringArm->bDoCollisionTest = false;
	SpringArm->TargetArmLength = 500.f;
	SpringArm->SetRelativeRotation(FRotator(-45, 90, 0));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->bUsePawnControlRotation = false; 
	Camera->SetupAttachment(SpringArm);

	const ConstructorHelpers::FObjectFinder<UInputAction> MoveActionFinder(TEXT("InputAction'/Game/Input/Move.Move'"));
	MoveAction = MoveActionFinder.Object;

	const ConstructorHelpers::FObjectFinder<UInputAction> LookActionFinder(TEXT("InputAction'/Game/Input/Look.Look'"));
	LookAction = LookActionFinder.Object;

	const ConstructorHelpers::FObjectFinder<UInputAction> ShootActionFinder(TEXT("InputAction'/Game/Input/Shoot.Shoot'"));
	ShootAction = ShootActionFinder.Object;

	static ConstructorHelpers::FObjectFinder<UBlueprint> ShotFinder(TEXT("Blueprint'/Game/Blueprints/BP_Shot.BP_Shot'"));
	ShotClass = ShotFinder.Object->GeneratedClass;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	ShotIndex = FMath::Rand();
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if(UEnhancedInputComponent* inputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
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
	rotation.Yaw += axisValue;
	SetActorRotation(rotation);
}


void APlayerPawn::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	AddActorWorldOffset(GetActorRotation().RotateVector(FVector(-MovementVector.X * Speed, MovementVector.Y * Speed, 0.f)));
}

void APlayerPawn::Shoot(const FInputActionValue& Value)
{
	FVector Location = GetActorLocation();
	FRotator Rotation = GetActorRotation();
	const FVector Offset = ShotOffsets[ShotIndex++ % 4];
	Location = Location + Rotation.RotateVector(Offset);
	Rotation = Rotation + FRotator(0, -90, 0);
	GetWorld()->SpawnActor(ShotClass, &Location, &Rotation, FActorSpawnParameters());
}

