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
		if (AmmoNum)
		{
			AmmoNum->TextDelegate.BindUFunction(this, "SetAmmoField");
			AmmoNum->SynchronizeProperties();
		}
		if (ScoreNum)
		{
			ScoreNum->TextDelegate.BindUFunction(this, "SetScoreField");
			ScoreNum->SynchronizeProperties();
		}
		if(HealthBar)
		{
			HealthBar->PercentDelegate.BindUFunction(this, "SetHealthBar");
			HealthBar->SynchronizeProperties();
		}
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
