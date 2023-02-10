// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerPawn.h"
#include "PlayerHud.generated.h"

/**
 * 
 */

UCLASS()
class PROG2COMP2_API UPlayerHud : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	UFUNCTION()
	FText SetAmmoField() const;
	UFUNCTION()
	FText SetScoreField() const;
	UFUNCTION()
	float SetHealthBar() const;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Essentials", meta = (ExposeOnSpawn = true))
		APlayerPawn* MyPlayer;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* HealthBar;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* AmmoNum;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* ScoreNum;
};
