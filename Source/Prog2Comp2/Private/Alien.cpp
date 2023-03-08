// Fill out your copyright notice in the Description page of Project Settings.


#include "Alien.h"
#include "Components/BoxComponent.h"
#include "GameplayTagContainer.h"
#include "Materials/Material.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AAlien::AAlien()
{
	PrimaryActorTick.bCanEverTick = true;

	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshFinder(TEXT("StaticMesh'/Game/Models/SpaceShip/SpaceShip.SpaceShip'"));
	const ConstructorHelpers::FObjectFinder<UMaterial> MeshMaterialFinder(TEXT("Material'/Game/Models/SpaceShip/MM_SpaceShip.MM_SpaceShip'"));

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	SetRootComponent(Collider);
	Collider->InitBoxExtent(FVector(10, 50, 50));

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(GetRootComponent());
	StaticMesh->SetRelativeScale3D(FVector(0.1f, 1.f, 1.f));
	StaticMesh->SetRelativeLocation(FVector(0.f, 0.f, -50));

	MovementSpeed = 350;
	RotationSpeed = 1.f;
	XKillPosition = -200.f;

	Tags.Add(FName("Enemy"));
}

void AAlien::BeginPlay()
{
	Super::BeginPlay();
	RotationSpeed = FMath::RandRange(0.5f, 1.5f);
	MovementSpeed += FMath::RandRange(0, 250);
}

void AAlien::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Move
	FVector newLocation = GetActorLocation();
	if (const APlayerController* pc = GetWorld()->GetFirstPlayerController())
	{
		const FVector pcLocation = pc->GetPawn()->GetActorLocation();
		FVector direction = pcLocation - newLocation;
		// normalize direction
		direction.Normalize();
		direction *= FVector(1.f, 1.f, 0.f);
		newLocation += direction * MovementSpeed * DeltaTime;
	}
	else
	{
		newLocation += FVector(-1, 0, 0) * MovementSpeed * DeltaTime;
	}
	SetActorLocation(newLocation);

	// Rotate
	SetActorRotation(GetActorRotation() + FRotator(0, RotationSpeed, 0));
}

void AAlien::DestroyTarget()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	this->Destroy();
}

