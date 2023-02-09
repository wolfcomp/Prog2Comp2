// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Shot.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshFinder(TEXT("StaticMesh'/Engine/BasicShapes/Cone.Cone'"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root"));
	SetRootComponent(Mesh);
	Mesh->SetStaticMesh(MeshFinder.Object);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Mesh);
	SpringArm->bDoCollisionTest = false;
	SpringArm->TargetArmLength = 500.f;
	SpringArm->SetRelativeRotation(FRotator(-45, 180, 0));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->bUsePawnControlRotation = false; 
	Camera->SetupAttachment(SpringArm);

	const ConstructorHelpers::FObjectFinder<UInputAction> MoveActionFinder(TEXT("InputAction'/Game/Input/Move.Move'"));
	MoveAction = MoveActionFinder.Object;

	const ConstructorHelpers::FObjectFinder<UInputAction> ShootActionFinder(TEXT("InputAction'/Game/Input/Shoot.Shoot'"));
	ShootAction = ShootActionFinder.Object;

	AutoPossessPlayer = EAutoReceiveInput::Player0;
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
	}
}

void APlayerPawn::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Move: %f, %f"), MovementVector.X * Speed, MovementVector.Y * Speed));

	AddActorWorldOffset(FVector(-MovementVector.Y * Speed, -MovementVector.X * Speed, 0.f));
}

void APlayerPawn::Shoot(const FInputActionValue& Value)
{
	const FVector Location = GetActorLocation();
	const FRotator Rotation = GetActorRotation();
	GetWorld()->SpawnActor(AShot::StaticClass(), &Location, &Rotation, FActorSpawnParameters());
}

