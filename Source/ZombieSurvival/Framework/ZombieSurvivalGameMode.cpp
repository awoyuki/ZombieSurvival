// Copyright Epic Games, Inc. All Rights Reserved.

#include "ZombieSurvivalGameMode.h"
#include "ZombieSurvivalPlayerController.h"
#include "ZombieSurvivalCharacter.h"
#include "UObject/ConstructorHelpers.h"

AZombieSurvivalGameMode::AZombieSurvivalGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AZombieSurvivalPlayerController::StaticClass();

}