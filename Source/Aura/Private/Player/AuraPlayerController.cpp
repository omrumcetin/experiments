// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "Core/AuraGameplayTags.h"
#include "Input/AuraEnhancedInputComponent.h"
#include "Interaction/AuraTargetInterface.h"

class UEnhancedInputLocalPlayerSubsystem;

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;

	MovementSpline = CreateDefaultSubobject<USplineComponent>("MovementSpline");
}

void AAuraPlayerController::PlayerTick(float DeltaSeconds)
{
	Super::PlayerTick(DeltaSeconds);

	CursorTrace();
	AutoRun();
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
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTagLeftMouseButton) || bTargeting)
	{
		if (!GetASC())
		{
			return;
		}
		GetASC()->AbilityInputTagHeld(InputTag);
		return;
	}
	
	FollowTime += GetWorld()->GetDeltaSeconds();
	if (CursorHit.bBlockingHit)
	{
		CachedDestination = CursorHit.ImpactPoint;
	}

	if (APawn* controlledPawn = GetPawn())
	{
		const FVector WorldDirection = (CachedDestination - controlledPawn->GetActorLocation()).GetSafeNormal();
		controlledPawn->AddMovementInput(WorldDirection);
	}

}

void AAuraPlayerController::AbilityInputReleased(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTagLeftMouseButton) || bTargeting)
	{
		if (!GetASC())
		{
			return;
		}
		GetASC()->AbilityInputTagReleased(InputTag);
		return;
	}

	APawn* controlledPawn = GetPawn();
	if (controlledPawn && FollowTime < ShortPressThreshold)
	{
		if (UNavigationPath* navPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, controlledPawn->GetActorLocation(), CachedDestination))
		{
			MovementSpline->ClearSplinePoints();
			for (const FVector& pointLoc : navPath->PathPoints)
			{
				MovementSpline->AddSplinePoint(pointLoc, ESplineCoordinateSpace::World);
				DrawDebugSphere(GetWorld(), pointLoc, 16.f, 16, FColor::Green, false, 5.f);
			}
			if (navPath->PathPoints.IsEmpty())
			{
				return;
			}
			CachedDestination = navPath->PathPoints[navPath->PathPoints.Num() - 1];
			bAutoRun = true;
		}		
	}
	FollowTime = 0;
	bTargeting = false;
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

	GetHitResultUnderCursor(ECC_Pawn, false, CursorHit);
	if (!CursorHit.bBlockingHit)
	{
		return;
	}

	LastActor = CurrentActor;
	CurrentActor = Cast<IAuraTargetInterface>(CursorHit.GetActor());

	if (LastActor != CurrentActor)
	{
		if (LastActor)
		{
			LastActor->UnhighlightActor();
		}
		if (CurrentActor)
		{
			CurrentActor->HightlightActor();
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

void AAuraPlayerController::AutoRun()
{
	if (!bAutoRun)
	{
		return;
	}

	if (APawn* controlledPawn = GetPawn())
	{
		const FVector locationOnSpline = MovementSpline->FindLocationClosestToWorldLocation(controlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector direction = MovementSpline->FindDirectionClosestToWorldLocation(locationOnSpline, ESplineCoordinateSpace::World);
		controlledPawn->AddMovementInput(direction);

		const float distanceToDestination = (locationOnSpline - CachedDestination).Length();
		if (distanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRun = false;
		}
	}
}
