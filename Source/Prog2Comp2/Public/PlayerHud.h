// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHud.generated.h"

/**
 * 
 */

class APlayerPawn;

UCLASS()
class PROG2COMP2_API UPlayerHud : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Essentials", meta = (ExposeOnSpawn = true))
		APlayerPawn* MyPlayer;
public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* HealthBar;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* AmmoNum;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* ScoreNum;
};
