// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IWeapon.generated.h"

/*
This class does not need to be modified.
Empty class for reflection system visibility.
Uses the UINTERFACE macro.
Inherits from UInterface.
*/
UINTERFACE(Blueprintable)
class ZOMBIESURVIVAL_API UIWeapon : public UInterface
{
	GENERATED_BODY()
};

/* Actual Interface declaration. */
class IIWeapon
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// Add interface function declarations here

	UFUNCTION()
	virtual void GetOwner(AZS_Player* &Owner);

	UFUNCTION()
	virtual void OnEquippedWeapon(AZS_Player* NewOwner, FWeaponDataStruct NewWeaponData);

};
