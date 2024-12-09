// Copyright Epic Games, Inc. All Rights Reserved.

#include "ZombieSurvivalGameMode.h"
#include "ZombieSurvivalPlayerController.h"
#include "ZombieSurvivalPlayerState.h"
#include "ZombieSurvivalGameState.h"
#include "ZombieSurvivalCharacter.h"
#include "UObject/ConstructorHelpers.h"

AZombieSurvivalGameMode::AZombieSurvivalGameMode()
{
	// use our custom PlayerController class
	GameStateClass = AZombieSurvivalGameState::StaticClass();
	PlayerStateClass = AZombieSurvivalPlayerState::StaticClass();
	PlayerControllerClass = AZombieSurvivalPlayerController::StaticClass();
}