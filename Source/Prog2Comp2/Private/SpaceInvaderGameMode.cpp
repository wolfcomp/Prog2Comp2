// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceInvaderGameMode.h"

#include "EnhancedInputSubsystems.h"
#include "PlayerPawn.h"
#include "InputMappingContext.h"

ASpaceInvaderGameMode::ASpaceInvaderGameMode()
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> inputMappingContextFinder(TEXT("/Game/Input/SpaceInvadersInputs.SpaceInvadersInputs"));
	InputMappingContext = inputMappingContextFinder.Object;
	static ConstructorHelpers::FObjectFinder<UBlueprint> playerPawnObject(TEXT("Blueprint'/Game/Blueprints/PlayerPawn.PlayerPawn'"));
	PlayerPawn = playerPawnObject.Object->GeneratedClass;
}


void ASpaceInvaderGameMode::HandleStartingNewPlayer_Implementation(APlayerController* newPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(newPlayer);

	/*if (!NewPlayer->GetPawn())
	{
		return;
	}*/

	if (APawn* tmpPawn = newPlayer->GetPawn())
	{
		tmpPawn->Destroy();
	}

	if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(newPlayer->GetLocalPlayer())) {
		subsystem->AddMappingContext(InputMappingContext, 0);
	}

	FActorSpawnParameters spawnInfo;
	spawnInfo.Owner = newPlayer;
	spawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	FTransform spawnTransform = FTransform();
	spawnTransform.SetLocation(FVector(0, 0, 50));
	const FVector tmpLoc = spawnTransform.GetLocation();
	const FRotator tmpRot = spawnTransform.GetRotation().Rotator();
	APlayerPawn* newController = Cast<APlayerPawn>(GetWorld()->SpawnActor(PlayerPawn, &tmpLoc, &tmpRot, spawnInfo));
	newPlayer->Possess(newController);
}
