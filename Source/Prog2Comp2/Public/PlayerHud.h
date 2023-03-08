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
	FText WinLoseText = FText::FromString("");
	virtual void NativeConstruct() override;
	UFUNCTION()
	FText SetAmmoField() const;
	UFUNCTION()
	FText SetScoreField() const;
	UFUNCTION()
	float SetHealthBar() const;
	UFUNCTION()
	FText LocSetWinLoseField() const;
	UFUNCTION()
	void SetWinLoseField(FText text, ESlateVisibility displayWinImage, ESlateVisibility displayText);
	UPROPERTY(EditAnywhere)
		APlayerPawn* MyPlayer = __nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* HealthBar = __nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* AmmoNum = __nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* ScoreNum = __nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* WinLoseTextTxt = __nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* WinImage = __nullptr;
};
