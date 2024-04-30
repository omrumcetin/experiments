// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AuraOverlayWidgetController.generated.h"

class UAuraUserWidget;


USTRUCT(BlueprintType)
struct FMessageContainerRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MessageContainer")
	FGameplayTag MessageTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MessageContainer")
	FText Message;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MessageContainer")
	TSubclassOf<UAuraUserWidget> MessageWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MessageContainer")
	TObjectPtr<UTexture2D> Image;
	
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMessageNotifySignature, FMessageContainerRow, Row);

UCLASS(BlueprintType, Blueprintable)
class AURA_API UAuraOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnHealthChanged;
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnManaChanged;
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnMaxManaChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Message")
	FOnMessageNotifySignature OnMessageNotify;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GAS|Attributes")
	TObjectPtr<UDataTable> MessageContainerDataTable;

private:
	template<typename T>
	T* GetDataTableRowByTag(const UDataTable* DataTable, const FGameplayTag& Tag) const;
};

template <typename T>
T* UAuraOverlayWidgetController::GetDataTableRowByTag(const UDataTable* DataTable, const FGameplayTag& Tag) const
{
	if (!DataTable)
	{
		return nullptr;
	}
	return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
}
