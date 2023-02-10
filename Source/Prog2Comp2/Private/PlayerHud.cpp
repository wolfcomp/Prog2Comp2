// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHud.h"
#include "PlayerPawn.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPlayerHud::NativeConstruct()
{
	if (MyPlayer)
	{
		//bind properties
		AmmoNum->TextDelegate.BindUFunction(this, "SetAmmoField");
		ScoreNum->TextDelegate.BindUFunction(this, "SetScoreField");
		HealthBar->PercentDelegate.BindUFunction(this, "SetHealthBar");

		//update them
		AmmoNum->SynchronizeProperties();
		ScoreNum->SynchronizeProperties();
		HealthBar->SynchronizeProperties();
	} else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "MyPlayer not valid");
	}
}

FText UPlayerHud::SetAmmoField() const
{
	return FText::FromString(FString::FromInt(MyPlayer->Ammo));
}

FText UPlayerHud::SetScoreField() const
{
	return FText::FromString(FString::FromInt(MyPlayer->Score));
}

float UPlayerHud::SetHealthBar() const
{
	return MyPlayer->CurrentHealth / MyPlayer->MaxHealth;
	//return 0.5;
}
