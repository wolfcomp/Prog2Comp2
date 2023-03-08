// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerPawn.h"
#include "GameFramework/Actor.h"
#include "AlienSpawner.generated.h"

class AAlien;

UCLASS()
class PROG2COMP2_API AAlienSpawner : public AActor
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float delta_time) override;
	AAlienSpawner();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Variables")
		float SpawnDelay;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Variables")
		int AlienSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Variables")
		float XSpawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Variables")
		float YSpawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Variables")
		int AlienKillsToWin;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Variables")
	    TSubclassOf<AAlien> BP_Alien;

	bool GameWon;

private:
	float InternalTimer;


    bool StopSpawning();
};
