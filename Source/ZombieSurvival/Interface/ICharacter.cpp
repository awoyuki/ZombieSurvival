// Fill out your copyright notice in the Description page of Project Settings.


#include "ICharacter.h"
#include "ZombieSurvival/Data/WeaponData.h"


// Add default functionality here for any IICharacter functions that are not pure virtual.

float IICharacter::GetMovementOffset()
{
	return 0.0f;
}

void IICharacter::OnPlayerMouseStart()
{
}

void IICharacter::OnPlayerMouseEnd()
{
}

void IICharacter::OnPlayerFiringWeapon(FWeaponDataStruct WeaponData)
{
}


void IICharacter::OnPlayerChangeWeapon()
{
}
