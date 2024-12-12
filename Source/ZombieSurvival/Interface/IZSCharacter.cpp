// Fill out your copyright notice in the Description page of Project Settings.


#include "IZSCharacter.h"

// Add default functionality here for any IIZSCharacter functions that are not pure virtual.

bool IIZSCharacter::IsDead_Implementation()
{
	return false;
}

AWeaponBase* IIZSCharacter::GetCurrentWeapon_Implementation()
{
	return nullptr;
}

void IIZSCharacter::PickUpWeapon_Implementation(UWeaponData* NewWeaponData)
{
}
