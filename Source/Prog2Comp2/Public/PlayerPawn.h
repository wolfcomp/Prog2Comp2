// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
//#include "Blueprint/UserWidgetBlueprint.h"
#include "PlayerHud.h"
#include "PlayerPawn.generated.h"

struct FInputActionValue;
//class UPlayerHud;

UCLASS()
class PROG2COMP2_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(const FInputActionValue& Value);

	void Shoot(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> PlayerHudTemplate = __nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
		float Speed = 20.0f;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputSystem)
		class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputSystem)
		UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputSystem)
		UInputAction* ShootAction;

	UPROPERTY(EditAnywhere)
		class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere)
		class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Ammo = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CurrentHealth = 50.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxHealth = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Score = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AShot> ShotClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FVector> ShotOffsets = { FVector(201.5, 220, 21.5), FVector(-201.5, 220, 119), FVector(201.5, 220, 119), FVector(-201.5, 220, 21.5) };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int ShotIndex = 0;

	UFUNCTION(BlueprintCallable)
	void Collide(AActor* other_actor);
private:
	UPROPERTY() UPlayerHud* PlayerHudWidget = __nullptr;
};
