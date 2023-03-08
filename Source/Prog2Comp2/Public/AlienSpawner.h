// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AlienSpawner.generated.h"

class AAlien;

struct Wave
{
	Wave(int s, int d)
	{
		Size = s;
		Difficulty = d;
	}
	int Size;
	int Difficulty;
};

UCLASS()
class PROG2COMP2_API AAlienSpawner : public AActor
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	AAlienSpawner();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Variables")
		TArray<int> WaveSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Variables")
		TArray<int> WaveDifficulty;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Variables")
		TArray<int> WaveSpawnRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Variables")
		float MinX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Variables")
		float MaxX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Variables")
		float MinY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Variables")
		float MaxY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Variables")
		int CurrentWave;

private:
	int LeftToSpawn;
	float Clock;

	void ChangeWave(int Wave);
	bool GameWon;

};
