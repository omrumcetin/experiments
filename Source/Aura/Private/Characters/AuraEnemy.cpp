// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AuraEnemy.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "UI/Widgets/AuraUserWidget.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Core/AuraGameplayTags.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Logging/StructuredLog.h"


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
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	UAuraAbilitySystemLibrary::InitializeDefaultAbilities(this, AbilitySystemComponent);
	
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
				UE_LOGFMT(LogTemp, Verbose, "FOnAttributeData streaming OldValue({OldValue}), NewValue({NewValue})", Data.OldValue, Data.NewValue);
			}
		);

		AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().EffectHitReact, EGameplayTagEventType::NewOrRemoved).AddLambda(
			[this](const FGameplayTag Tag, int32 NewCount)
			{
				if (NewCount > 0)
				{
					GetCharacterMovement()->MaxWalkSpeed = 0.f;
				}
				else
				{
					GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
				}
				
			});

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
	
