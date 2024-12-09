// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "ZombieSurvivalCharacter.h"
#include "ZombieSurvivalPlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/World.h"

AZombieSurvivalPlayerController::AZombieSurvivalPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AZombieSurvivalPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

}

void AZombieSurvivalPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


