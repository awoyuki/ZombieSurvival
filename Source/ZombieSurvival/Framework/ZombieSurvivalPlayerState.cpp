// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieSurvivalPlayerState.h"

void AZombieSurvivalPlayerState::CalculateAmmo(EWeaponType WeaponType, int Value)
{
	switch (WeaponType)
	{
	case EWeaponType::AssaultRifle:
		TotalRifleAmmo = fmax(TotalRifleAmmo - Value, 0);
		break;
	case EWeaponType::GrenadeLauncher:
		TotalGrenadeAmmo = fmax(TotalGrenadeAmmo - Value, 0);
		break;
	}
}

int AZombieSurvivalPlayerState::GetTotalAmmo(EWeaponType WeaponType)
{
	return 0;
}
