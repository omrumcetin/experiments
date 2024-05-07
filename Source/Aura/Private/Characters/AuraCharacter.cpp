// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/States/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeRotation(FRotator(45.f, 0.f, 0.f));
	SpringArm->TargetArmLength = 750.f;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArm);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ability actor info for server
	InitAbilityActorInfo();
	GiveAbilitiesToCharacter();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info for server
	InitAbilityActorInfo();
}

uint32 AAuraCharacter::GetPlayerLevel()
{
	AAuraPlayerState* ps = GetPlayerState<AAuraPlayerState>();
	return ps->GetPlayerLevel();
}

void AAuraCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState* ps = GetPlayerState<AAuraPlayerState>();
	check(ps);
	ps->GetAbilitySystemComponent()->InitAbilityActorInfo(ps, this);
	AbilitySystemComponent = ps->GetAbilitySystemComponent();
	AttributeSet = ps->GetAttributeSet();

	if (AAuraPlayerController* pc = Cast<AAuraPlayerController>(GetController()))
	{
		if (AAuraHUD* hud = Cast<AAuraHUD>(pc->GetHUD()))
		{
			hud->InitOverlay(pc, ps, AbilitySystemComponent, AttributeSet);
		}
	}

	InitializePrimaryAttributes();
	InitializeSecondaryAttributes();
	InitializeVitalAttributes();
}
