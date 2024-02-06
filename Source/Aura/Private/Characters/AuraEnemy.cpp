// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AuraEnemy.h"

void AAuraEnemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
}

void AAuraEnemy::HightlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
}

void AAuraEnemy::UnhighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
}
	
