// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AuraEnemy.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "UI/Widgets/AuraUserWidget.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

	OverheadWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("WidgetComponent");
	OverheadWidgetComponent->SetupAttachment(GetRootComponent());
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();
	
	if (UAuraUserWidget* auraWidget = Cast<UAuraUserWidget>(OverheadWidgetComponent->GetUserWidgetObject()))
	{
		auraWidget->SetWidgetController(this);
	}

	if (const UAuraAttributeSet* auraAS = Cast<UAuraAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(auraAS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(auraAS->GetMaxHealthAttribute()).AddLambda(
			[this, auraAS](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(auraAS->GetMaxHealth());
				UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Why FOnAttributeChangeData is null for %s: %f, %f"), *Data.Attribute.AttributeName, Data.NewValue, Data.OldValue), true, true, FLinearColor::Red, 4);
			}
		);

		OnHealthChanged.Broadcast(auraAS->GetHealth());
		OnMaxHealthChanged.Broadcast(auraAS->GetMaxHealth());
	}
}

void AAuraEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->PostAbilityActorInfoSet();
	
	InitializeDefaultAttributes();
}

void AAuraEnemy::InitializeDefaultAttributes() const
{
	UAuraAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}

void AAuraEnemy::HightlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
}

void AAuraEnemy::UnhighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
}
	
