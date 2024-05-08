// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Core/AuraGameplayTags.h"
#include "Input/AuraEnhancedInputComponent.h"
#include "Interaction/AuraTargetInterface.h"

class UEnhancedInputLocalPlayerSubsystem;

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaSeconds)
{
	Super::PlayerTick(DeltaSeconds);

	CursorTrace();
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (subsystem)
	{
		subsystem->AddMappingContext(AuraContext,  0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI inputModeData;
	inputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	inputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(inputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UAuraEnhancedInputComponent* enhancedInputComponent = CastChecked<UAuraEnhancedInputComponent>(InputComponent);
	enhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);

	enhancedInputComponent->BindAbilityInputActions(InputConfig, this, &ThisClass::AbilityInputPressed, &ThisClass::AbilityInputHeld, &ThisClass::AbilityInputReleased);
}

void AAuraPlayerController::AbilityInputPressed(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTagLeftMouseButton))
	{
		bTargeting = CurrentActor ? true : false;
		bAutoRun = false;
	}
}

void AAuraPlayerController::AbilityInputHeld(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTagLeftMouseButton))
	{
		if (!GetASC())
		{
			return;
		}
		GetASC()->AbilityInputTagHeld(InputTag);
		return;
	}

	// If any target on the mouse click use ability!!
	if (bTargeting)
	{
		if (!GetASC())
		{
			return;
		}
		GetASC()->AbilityInputTagHeld(InputTag);
	}
	else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();
		FHitResult hitResult;
		if (GetHitResultUnderCursor(ECC_Visibility, false, hitResult))
		{
			CachedDestination = hitResult.ImpactPoint;
		}

		if (APawn* controlledPawn = GetPawn())
		{
			const FVector WorldDirection = (CachedDestination - controlledPawn->GetActorLocation()).GetSafeNormal();
			controlledPawn->AddMovementInput(WorldDirection);
		}
	}
}

void AAuraPlayerController::AbilityInputReleased(FGameplayTag InputTag)
{
	if (!GetASC())
	{
		return;
	}
	GetASC()->AbilityInputTagReleased(InputTag);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D inputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator rotation = GetControlRotation();
	const FRotator yawRotation(0, rotation.Yaw, 0.f);

	const FVector forwardDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);
	const FVector rightDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* controlledPawn = GetPawn<APawn>())
	{
		controlledPawn->AddMovementInput(forwardDirection, inputAxisVector.Y);
		controlledPawn->AddMovementInput(rightDirection, inputAxisVector.X);
	}
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult cursorHit;
	GetHitResultUnderCursor(ECC_Pawn, false, cursorHit);
	if (!cursorHit.bBlockingHit)
	{
		return;
	}

	LastActor = CurrentActor;
	CurrentActor = Cast<IAuraTargetInterface>(cursorHit.GetActor());

	/**
	 * Line trace from cursor and there is possible scenarios.
	 * A. LastActor is null and CurrentActor is null
	 * - Do nothing.
	 * B. LastActor is valid and CurrentActor is null
	 * - Unhighlight LastActor.
	 * C. LastActor is null and CurrentActor is valid
	 * - Highlight CurrentActor
	 * D. LastActor is valid and CurrentActor is valid
	 * - Unhighlight LastActor, Hightlight CurrentActor
	 */

	if (!LastActor)
	{
		if (CurrentActor)
		{
			CurrentActor->HightlightActor();
		}
	}
	else
	{
		if (CurrentActor)
		{
			if (CurrentActor != LastActor)
			{
				LastActor->UnhighlightActor();
				CurrentActor->HightlightActor();
			}
		}
		else
		{
			LastActor->UnhighlightActor();
		}
	}
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (!AuraAbilitySystemComponent)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()));
	}
	return AuraAbilitySystemComponent;
}
