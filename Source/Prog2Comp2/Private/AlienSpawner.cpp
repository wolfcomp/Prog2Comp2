// Fill out your copyright notice in the Description page of Project Settings.


#include "AlienSpawner.h"
#include "Alien.h"
#include "PlayerPawn.h"

// Sets default values
AAlienSpawner::AAlienSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	XSpawnLocation = FMath::RandRange(0, 500);
	YSpawnLocation = FMath::RandRange(0, 500);

	SpawnDelay = 5.f;
	GameWon = false;

}

// Called when the game starts or when spawned
void AAlienSpawner::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AAlienSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	InternalTimer += DeltaTime;
	StopSpawning();

	if (!GameWon)
	{
		FVector location = GetActorLocation();
		location.Z = 130;

		//Spawning
		if (InternalTimer > SpawnDelay)
		{
			if(AAlien* Target = GetWorld()->SpawnActor<AAlien>(AAlien::StaticClass(), location, FRotator::ZeroRotator))
			{
			    Target->AlienMoveSpeed = AlienSpeed + FMath::RandRange(-100, 300);
			}
			InternalTimer = 0.f;
		}
	}
	
}


void AAlienSpawner::StopSpawning()
{
	if (const APlayerController* pc = GetWorld()->GetFirstPlayerController())
	{
		if (const APawn* pcPawn = pc->GetPawn())
		{
			const APlayerPawn* playerPawn = Cast<APlayerPawn>(pcPawn);
			if (playerPawn->Score > AlienKillsToWin)
			{
				GameWon = true;
			}
            else
            {
                GameWon = false;
            }
		}
	}
}