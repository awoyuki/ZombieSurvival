// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Components/DataController.h"
#include "ZombieSurvivalGameState.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESURVIVAL_API AZombieSurvivalGameState : public AGameState
{
	GENERATED_BODY()

public:
	AZombieSurvivalGameState();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UDataController* DataController;
};
