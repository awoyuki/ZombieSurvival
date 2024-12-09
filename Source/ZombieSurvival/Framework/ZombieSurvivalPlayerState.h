// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ZombieSurvival/Data/WeaponData.h"
#include "ZombieSurvivalPlayerState.generated.h"


UENUM(BlueprintType)
enum class EPlayerStatus : uint8
{
	Default UMETA(DisplayName = "Default"),
	Stunned UMETA(DisplayName = "Stunned")
};

/**
 * 
 */
UCLASS()
class ZOMBIESURVIVAL_API AZombieSurvivalPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EPlayerStatus PlayerStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int TotalRifleAmmo = 90;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int TotalGrenadeAmmo = 12;

	void CalculateAmmo(EWeaponType WeaponType, int Value);
	int GetTotalAmmo(EWeaponType WeaponType);
};
