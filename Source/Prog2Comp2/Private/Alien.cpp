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

	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshFinder(TEXT("StaticMesh'/Game/Models/Alien/AlienEnemy.AlienEnemy'"));
	const ConstructorHelpers::FObjectFinder<UMaterial> MeshMaterialFinder(TEXT("Material'/Game/Models/Alien/MM_Alien.MM_Alien'"));

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	SetRootComponent(Collider);
	Collider->InitBoxExtent(FVector(10, 50, 50));
	Collider->SetRelativeScale3D(FVector(2,2,2));

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(GetRootComponent());
	StaticMesh->SetRelativeLocation(FVector(0.f, 0.f, -50));
	StaticMesh->SetStaticMesh(MeshFinder.Object);
	StaticMesh->SetMaterial(0, MeshMaterialFinder.Object);

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
		if (const APawn* pcPawn = pc->GetPawn())
		{
			const FVector pcLocation = pcPawn->GetActorLocation();
			FVector direction = pcLocation - newLocation;
			// normalize direction
			direction.Normalize();
			direction *= FVector(1.f, 1.f, 0.f);
			newLocation += direction * MovementSpeed * DeltaTime;
			GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Red, FString::Printf(TEXT("newLocation: %s"), *newLocation.ToString()));
		}
		else
		{
			goto oldCalc;
		}
	}
	else
	{
	oldCalc:
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

