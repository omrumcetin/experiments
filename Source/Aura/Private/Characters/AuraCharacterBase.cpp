// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AuraCharacterBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"

AAuraCharacterBase::AAuraCharacterBase()
{
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// Called when the game starts or when spawned
void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraCharacterBase::GiveAbilitiesToCharacter()
{
	if (!HasAuthority()) return;
	
	UAuraAbilitySystemComponent* auraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	auraASC->AddCharacterAbilities(StartupAbilities);
}

void AAuraCharacterBase::InitializePrimaryAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributesEffectClass, 1.f);
}

void AAuraCharacterBase::InitializeSecondaryAttributes() const
{
	ApplyEffectToSelf(DefaultSecondaryAttributesEffectClass, 1.f);
}

void AAuraCharacterBase::InitializeVitalAttributes() const
{
	ApplyEffectToSelf(DefaultVitalAttributesEffectClass, 1.f);
}

void AAuraCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, uint32 Level) const
{
	FGameplayEffectContextHandle EffectContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle GameplayEffectSpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, EffectContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*GameplayEffectSpecHandle.Data.Get());
}

