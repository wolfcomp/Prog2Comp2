// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "SpaceInvaderGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROG2COMP2_API ASpaceInvaderGameMode : public AGameMode
{
	GENERATED_BODY()

	ASpaceInvaderGameMode();

public:
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputSystem)
		class UInputMappingContext* InputMappingContext;
};
