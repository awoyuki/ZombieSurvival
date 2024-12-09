// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "ZombieSurvivalCharacter.h"
#include "ZombieSurvival/Interface/ICharacter.h"
#include "ZombieSurvivalPlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;

UCLASS()
class AZombieSurvivalPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AZombieSurvivalPlayerController();


protected:

	// To add mapping context
	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override;


};


