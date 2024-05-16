// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "AuraProjectile.generated.h"

class UNiagaraSystem;
class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class AURA_API AAuraProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAuraProjectile();

	UPROPERTY()
	FGameplayEffectSpecHandle DamageEffectSpecHandle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	TObjectPtr<USphereComponent> SphereCollision;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraSystem> ImpactEffect;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> LoopingSound;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> ImpactSound;

private:
	UPROPERTY()
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
	UPROPERTY()
	TObjectPtr<UAudioComponent> AudioComponent;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool BFromSweep, const FHitResult& SweepResult);
	void PlayImpactEffects();

	uint32 bHit : 1 {false};

};
