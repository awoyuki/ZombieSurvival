// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ICharacter.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UICharacter : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ZOMBIESURVIVAL_API IICharacter
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION()
	virtual float GetMovementOffset();

	UFUNCTION()
	virtual void OnPlayerMouseStart();

	UFUNCTION()
	virtual void OnPlayerMouseEnd();

	UFUNCTION()
	virtual void OnPlayerFiringWeapon(FWeaponDataStruct WeaponData);

	UFUNCTION()
	virtual void OnPlayerChangeWeapon();


};
