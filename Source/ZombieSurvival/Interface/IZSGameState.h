// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IZSGameState.generated.h"


UENUM(BlueprintType)
enum class EGameStatus : uint8
{
	None UMETA(DisplayName = "None"),
	Inprogress UMETA(DisplayName = "Inprogress"),
	Loading UMETA(DisplayName = "Loading"),
	Lose UMETA(DisplayName = "Lose"),
	Win UMETA(DisplayName = "Win"),
	QuitGame UMETA(DisplayName = "QuitGame")
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UIZSGameState : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ZOMBIESURVIVAL_API IIZSGameState
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Default ZSGameState")
	void GetGameStatus(EGameStatus& GameStatus);
	virtual void GetGameStatus_Implementation(EGameStatus &GameStatus);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Default ZSGameState")
	void SetGameStatus(EGameStatus NewStatus);
	virtual void SetGameStatus_Implementation(EGameStatus NewStatus);


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Default ZSGameState")
	void LoadNextZone();
	virtual void LoadNextZone_Implementation();
};
