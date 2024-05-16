// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraFireBolt.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actors/AuraProjectile.h"
#include "Core/AuraGameplayTags.h"
#include "Interaction/AuraCombatInterface.h"

void UAuraFireBolt::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UAuraFireBolt::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer)
	{
		return;
	}

	if (IAuraCombatInterface* combatInterface = Cast<IAuraCombatInterface>(GetAvatarActorFromActorInfo()))
	{

		FVector socketLocation = combatInterface->GetCombatSocketLocation();
		FRotator rotation = (ProjectileTargetLocation - socketLocation).Rotation();
		rotation.Pitch = 0.f;

		FTransform spawnTransform;
		socketLocation.Z -= 20;
		spawnTransform.SetLocation(socketLocation);
		spawnTransform.SetRotation(rotation.Quaternion());

		AAuraProjectile* projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,
			spawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		const UAbilitySystemComponent* sourceASC = GetAbilitySystemComponentFromActorInfo();
		check(DamageEffectClass);
		const FGameplayEffectSpecHandle damageSpec = sourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), sourceASC->MakeEffectContext());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(damageSpec, FAuraGameplayTags::Get().DataDamage, Damage.GetValueAtLevel(GetAbilityLevel()));
		projectile->DamageEffectSpecHandle = damageSpec;

		projectile->FinishSpawning(spawnTransform);
	}
}
