// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shot.generated.h"

class UCapsuleComponent;
class UNiagaraSystem;
class UNiagaraComponent;
class USpotLightComponent;
class UPointLightComponent;

UCLASS()
class PROG2COMP2_API AShot : public AActor
{
	GENERATED_BODY()

	bool exploded;

	float ExplosionIntensity() const;
	
public:
	// Sets default values for this actor's properties
	AShot();
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USoundWave* AlienDeathSound;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float delta_time) override;
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* overlapped_component, AActor* other_actor, UPrimitiveComponent* other_comp, int32 other_body_index, bool b_from_sweep, const FHitResult& sweep_result);

	UFUNCTION(BlueprintCallable)
		void Explode(AActor* other_actor);

	UPROPERTY(VisibleAnywhere)
		float Time = 0.0f;

	UPROPERTY(EditAnywhere)
		float LifeTime = 2.5f;

	UPROPERTY(EditAnywhere)
		float LightFadeTime = 1.0f;

	UPROPERTY(EditAnywhere)
		float ExplosionDuration = 2.0f;
	
	UPROPERTY(EditAnywhere)
		float Speed = 80.0f;

	UPROPERTY(EditAnywhere)
		float FadeDuration = 1.0f;

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere)
	USpotLightComponent* Light1;

	UPROPERTY(VisibleAnywhere)
	UPointLightComponent* Light2;

	UPROPERTY(VisibleAnywhere)
	UNiagaraSystem* ShotEffect;

	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* ShotEffectComponent;

	UPROPERTY(VisibleAnywhere)
	UNiagaraSystem* ExplosionEffect;

	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* ExplosionEffectComponent;
};
