// Fill out your copyright notice in the Description page of Project Settings.


#include "Alien.h"
#include "Components/BoxComponent.h"
#include "GameplayTagContainer.h"
#include "Materials/Material.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AAlien::AAlien()
{
	PrimaryActorTick.bCanEverTick = true;

	const ConstructorHelpers::FObjectFinder<UStaticMesh> meshFinder(TEXT("StaticMesh'/Game/Models/Alien/AlienEnemy.AlienEnemy'"));
	const ConstructorHelpers::FObjectFinder<UMaterial> meshMaterialFinder(TEXT("Material'/Game/Models/Alien/MM_Alien.MM_Alien'"));

	//Constructing the collider 
	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	SetRootComponent(Collider);
	Collider->InitBoxExtent(FVector(10, 50, 50));
	Collider->SetRelativeScale3D(FVector(2,2,2));

	//Constructing the object mesh
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(GetRootComponent());
	StaticMesh->SetRelativeLocation(FVector(0.f, 0.f, -50));
	StaticMesh->SetStaticMesh(meshFinder.Object);
	StaticMesh->SetMaterial(0, meshMaterialFinder.Object);

	//Variables for the alien object
	AlienMoveSpeed = 1;
	RotationSpeed = 1.f;
	XKillPosition = -200.f;
	Tags.Add(FName("Enemy"));
}

void AAlien::BeginPlay()
{
	Super::BeginPlay();
	RotationSpeed = FMath::RandRange(0.5f, 1.5f);
}

void AAlien::Tick(float delta_time)
{
	Super::Tick(delta_time);

	// Moving the alien
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
			const FVector offsetLocation = direction * AlienMoveSpeed * delta_time;
			newLocation += offsetLocation;
		}
		else
		{
			goto defaultBehaviour;
		}
	}
	else
	{
	defaultBehaviour:
		newLocation += FVector(-1, 0, 0) * AlienMoveSpeed * delta_time;
	}
	SetActorLocation(newLocation);

	// Rotate
	SetActorRotation(GetActorRotation() + FRotator(0, RotationSpeed, 0));
}
