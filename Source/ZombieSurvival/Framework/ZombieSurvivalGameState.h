// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "ZombieSurvival/Interface/IZSGameState.h"
#include "ZombieSurvivalGameState.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameStatusDelegate, EGameStatus, NewStatus);

/**
 * 
 */

UCLASS()
class ZOMBIESURVIVAL_API AZombieSurvivalGameState : public AGameState, public IIZSGameState
{
	GENERATED_BODY()

	int CurrentLevelIndex = 1;

protected:

	virtual void BeginPlay() override;

public:

	AZombieSurvivalGameState();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString LevelPrefix = "Zone";

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int MaxLevelCount = 4;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EGameStatus CurrentGameStatus;

	virtual void GetGameStatus_Implementation(EGameStatus& GameStatus) override;
	
	virtual void SetGameStatus_Implementation(EGameStatus NewStatus) override;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Default ZSGameState")
	FGameStatusDelegate OnGameStatusChanged;

	virtual void LoadNextZone_Implementation() override;

};
