// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IZSCharacter.generated.h"

class UWeaponData;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIZSCharacter : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ZOMBIESURVIVAL_API IIZSCharacter
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Player Interface")
	bool IsDead();
	virtual bool IsDead_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Player Interface")
	class AWeaponBase* GetCurrentWeapon();
	virtual class AWeaponBase* GetCurrentWeapon_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Player Interface")
	void PickUpWeapon(UWeaponData* NewWeaponData);
	virtual void PickUpWeapon_Implementation(UWeaponData* NewWeaponData);

};
