// Fill out your copyright notice in the Description page of Project Settings.


#include "AlienSpawner.h"
#include "Alien.h"

// Sets default values
AAlienSpawner::AAlienSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MinX = 2000;
	MaxX = 2200;

	MinY = -400;
	MaxY = 400;

	WaveSize.Add(10);
	WaveSize.Add(15);
	WaveSize.Add(20);

	WaveDifficulty.Add(0.5);
	WaveDifficulty.Add(1);
	WaveDifficulty.Add(2);

	WaveSpawnRate.Add(2);
	WaveSpawnRate.Add(1);
	WaveSpawnRate.Add(0.5);

	GameWon = false;

}

// Called when the game starts or when spawned
void AAlienSpawner::BeginPlay()
{
	Super::BeginPlay();

	CurrentWave = 1;
	LeftToSpawn = WaveSize[CurrentWave - 1];
	
}

// Called every frame
void AAlienSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Clock += DeltaTime;

	if (Clock > WaveSpawnRate[CurrentWave - 1] && !GameWon)
	{
		Clock = 0.f;
		FVector location = FVector(FMath::RandRange(MinX, MaxX), FMath::RandRange(MinY, MaxY), 80);

		//Spawning
		AActor* Actor = GetWorld()->SpawnActor<AActor>(BP_Alien, location, FRotator::ZeroRotator);
		AAlien* Target = Cast<AAlien>(Actor);
		Target->MovementSpeed *= WaveDifficulty[CurrentWave - 1];
		LeftToSpawn--;
		if (LeftToSpawn <= 0)
		{
			ChangeWave(CurrentWave + 1);
		}
	}

}

void AAlienSpawner::ChangeWave(int Wave)
{
	if (WaveSize.Num() < Wave)
	{
		//Game Won
		GameWon = true;
		return;
	}

	CurrentWave = Wave;
	LeftToSpawn = WaveSize[CurrentWave - 1];
}

