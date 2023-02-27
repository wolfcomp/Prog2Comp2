// Fill out your copyright notice in the Description page of Project Settings.


#include "Alien.h"
#include "Components/BoxComponent.h"
#include "GameplayTagContainer.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AAlien::AAlien()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
}

// Called when the game starts or when spawned
void AAlien::BeginPlay()
{
	Super::BeginPlay();
	RotationSpeed = FMath::RandRange(0.5f, 1.5f);
	MovementSpeed += FMath::RandRange(0, 250);
	
}

// Called every frame
void AAlien::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Move
	FVector NewLocation = GetActorLocation();
	NewLocation += FVector(-1, 0, 0) * MovementSpeed * DeltaTime;
	SetActorLocation(NewLocation);

	// Rotate
	SetActorRotation(GetActorRotation() + FRotator(0, RotationSpeed, 0));

}

void AAlien::DestroyTarget()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	this->Destroy();
}

