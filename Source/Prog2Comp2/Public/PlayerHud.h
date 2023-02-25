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
public:
	virtual void NativeConstruct() override;
	UFUNCTION()
	FText SetAmmoField() const;
	UFUNCTION()
	FText SetScoreField() const;
	UFUNCTION()
	float SetHealthBar() const;
	UPROPERTY(EditAnywhere)
		APlayerPawn* MyPlayer = __nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* HealthBar = __nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* AmmoNum = __nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* ScoreNum = __nullptr;
};
