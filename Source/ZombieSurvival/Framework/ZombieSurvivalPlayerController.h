// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
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

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;
	
	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* InputMoveAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* InputMouseAction;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Property)
	double AngularDistanceBetweenPlayerAndCursor();

	/** Chacacter Comunication */
	IICharacter* ICharacter;

protected:

	virtual void SetupInputComponent() override;

	// To add mapping context
	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override;

	void OnPlayerMove(const FInputActionValue& value);

	void OnPlayerMouseStart();

	void OnPlayerMouseEnd();

	void FixPlayerRotation();

	FVector GetMouseLocation();

};


