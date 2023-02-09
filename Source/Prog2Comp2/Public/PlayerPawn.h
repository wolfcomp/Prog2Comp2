// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

struct FInputActionValue;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	float Speed = 20.0f;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputSystem)
		class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputSystem)
		class UInputAction* ShootAction;

	UPROPERTY(EditAnywhere)
		class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere)
		class UCameraComponent* Camera;
};
