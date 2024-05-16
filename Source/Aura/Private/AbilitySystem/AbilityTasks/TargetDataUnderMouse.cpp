// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"

// this is Static Factory
UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* myObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	return myObj;
}

void UTargetDataUnderMouse::Activate()
{
	if (IsLocallyControlled())
	{
		SendMouseCursorData();
		return;
	}
	
	// We need to listen our call through SendMouseCursorData casted by client.
	const FGameplayAbilitySpecHandle specHandle = GetAbilitySpecHandle();
	const FPredictionKey activationPredictionKey = GetActivationPredictionKey();
	AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(specHandle, activationPredictionKey).AddUObject(this, &ThisClass::OnTargetDataReplicatedCallback);
	const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(specHandle, activationPredictionKey);
	if (!bCalledDelegate)
	{
		SetWaitingOnRemotePlayerData();
	}
}

void UTargetDataUnderMouse::SendMouseCursorData()
{
	FScopedPredictionWindow scopedPrediction(AbilitySystemComponent.Get());
	//APlayerController* pc = Ability->GetCurrentActorInfo()->PlayerController.Get();
	APlayerController* pc = Cast<APlayerController>(GetAvatarActor()->GetInstigatorController());
	FHitResult cursorHit;
	pc->GetHitResultUnderCursor(ECC_Visibility, false, cursorHit);

	FGameplayAbilityTargetData_SingleTargetHit* data = new FGameplayAbilityTargetData_SingleTargetHit();
	data->HitResult = cursorHit;

	FGameplayAbilityTargetDataHandle dataHandle;
	dataHandle.Add(data);

	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		dataHandle,
		FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey);

	// Ensure ability still active.
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidDataDelegate.Broadcast(dataHandle);
	}
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(
	const FGameplayAbilityTargetDataHandle& GameplayAbilityTargetDataHandle, FGameplayTag GameplayTag)
{
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidDataDelegate.Broadcast(GameplayAbilityTargetDataHandle);
	}
}
