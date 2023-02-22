// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceInvaderGameMode.h"

#include "EnhancedInputSubsystems.h"
#include "PlayerPawn.h"
#include "InputMappingContext.h"

ASpaceInvaderGameMode::ASpaceInvaderGameMode()
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextFinder(TEXT("/Game/Input/SpaceInvadersInputs.SpaceInvadersInputs"));
	InputMappingContext = InputMappingContextFinder.Object;
	static ConstructorHelpers::FObjectFinder<UBlueprint> PlayerPawnObject(TEXT("Blueprint'/Game/Blueprints/PlayerPawn.PlayerPawn'"));
	PlayerPawn = PlayerPawnObject.Object->GeneratedClass;
}


void ASpaceInvaderGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	/*if (!NewPlayer->GetPawn())
	{
		return;
	}*/

	if (APawn* TmpPawn = NewPlayer->GetPawn())
	{
		TmpPawn->Destroy();
	}

	if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(NewPlayer->GetLocalPlayer())) {
		subsystem->AddMappingContext(InputMappingContext, 0);
	}

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Owner = NewPlayer;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	FTransform SpawnTransform = FTransform();
	SpawnTransform.SetLocation(FVector(0, 0, 50));
	const FVector TmpLoc = SpawnTransform.GetLocation();
	const FRotator TmpRot = SpawnTransform.GetRotation().Rotator();
	APlayerPawn* NewController = Cast<APlayerPawn>(GetWorld()->SpawnActor(PlayerPawn, &TmpLoc, &TmpRot, SpawnInfo));
	NewPlayer->Possess(NewController);
}
