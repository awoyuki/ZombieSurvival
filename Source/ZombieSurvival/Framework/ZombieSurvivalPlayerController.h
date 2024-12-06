// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "ZombieSurvivalCharacter.h"
#include "ZombieSurvivalPlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;

UCLASS()
class AZombieSurvivalPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AZombieSurvivalPlayerController();

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;
	
	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* InputMoveAction;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Property)
	double AngularDistanceBetweenPlayerAndCursor();

protected:

	//Movementoffset base on Camera
	float MovementOffset = 0;

	virtual void SetupInputComponent() override;

	// To add mapping context
	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override;

	void OnPlayerMove(const FInputActionValue& value);

	void FixPlayerRotation();

	FVector GetMouseLocation();

};


