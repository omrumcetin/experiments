// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouse.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetDataUnderMouseSignature, const FGameplayAbilityTargetDataHandle&, DataHandle);


UCLASS()
class AURA_API UTargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta=(DisplayName="TargetDataUnderMouse", HidePin="OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = true))
	static UTargetDataUnderMouse* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility);
	
	virtual void Activate() override;

protected:
	UPROPERTY(BlueprintAssignable, meta=(DisplayName="Valid Data"))
	FOnTargetDataUnderMouseSignature ValidDataDelegate;

	void SendMouseCursorData();
	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& GameplayAbilityTargetDataHandle, FGameplayTag GameplayTag);
};
