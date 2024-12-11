// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieSurvivalPlayerState.h"



float AZombieSurvivalPlayerState::GetHealth()
{
	return Health;
}

float AZombieSurvivalPlayerState::GetMaxHealth()
{
	return MaxHealth;
}

void AZombieSurvivalPlayerState::SetHealth(float NewHeath)
{
	Health = FMath::Min(NewHeath, MaxHealth);
	OnHealthChanged.Broadcast(NewHeath);
}

void AZombieSurvivalPlayerState::SetMaxHealth(float NewMaxHeath)
{
	MaxHealth = NewMaxHeath;
}

EPlayerStatus AZombieSurvivalPlayerState::GetPlayerStatus()
{
	return PlayerStatus;
}


void AZombieSurvivalPlayerState::SetPlayerStatus(EPlayerStatus NewStatus)
{
	PlayerStatus = NewStatus;
}

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
	OnAmmoChanged.Broadcast(TotalRifleAmmo, TotalGrenadeAmmo);
}

int AZombieSurvivalPlayerState::GetTotalAmmo(EWeaponType WeaponType)
{
	switch (WeaponType)
	{
		case EWeaponType::AssaultRifle:
			return TotalRifleAmmo;
		case EWeaponType::GrenadeLauncher:
			return TotalGrenadeAmmo; 
	}
	return 0;
}

void AZombieSurvivalPlayerState::SetTotalAmmo(EWeaponType WeaponType, int Value, const bool bAdd)
{
	switch (WeaponType)
	{
	case EWeaponType::AssaultRifle:
		TotalRifleAmmo = bAdd ? TotalRifleAmmo + Value : Value;
		break;
	case EWeaponType::GrenadeLauncher:
		TotalGrenadeAmmo = bAdd ? TotalGrenadeAmmo + Value : Value;
		break;
	}
	OnAmmoChanged.Broadcast(TotalRifleAmmo, TotalGrenadeAmmo);
}
