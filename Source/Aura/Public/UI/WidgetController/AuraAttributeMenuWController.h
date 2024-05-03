// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AuraAttributeMenuWController.generated.h"


class UAttributeInfo;
struct FAuraAttributeInfo;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FAuraAttributeInfo&, Info);

UCLASS(BlueprintType, Blueprintable)
class AURA_API UAuraAttributeMenuWController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	virtual void BindCallbacksToDependencies() override;
	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues() override;

	UPROPERTY(BlueprintAssignable)
	FAttributeInfoSignature AttributeInfoDelegate;

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfo> AttributeInfo;
};
