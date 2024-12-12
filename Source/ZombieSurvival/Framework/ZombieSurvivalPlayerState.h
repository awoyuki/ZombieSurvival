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
	Stunned UMETA(DisplayName = "Stunned"),
	Dead UMETA(DisplayName = "Dead")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHealthDelegate, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAmmoDelegate, float, NewRilfeAmmo, float, NewGrenadeAmmo);

/**
 * 
 */
UCLASS()
class ZOMBIESURVIVAL_API AZombieSurvivalPlayerState : public APlayerState
{
	GENERATED_BODY()

	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Stored Data")
	int Health = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Stored Data")
	int MaxHealth = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Stored Data")
	EPlayerStatus PlayerStatus;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Stored Data")
	int TotalRifleAmmo = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Stored Data")
	int TotalGrenadeAmmo = 0;


public:

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Player Stored Data")
	FHealthDelegate OnHealthChanged;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Player Stored Data")
	FAmmoDelegate OnAmmoChanged;

	UFUNCTION(BlueprintCallable, Category="Player Stored Data", BlueprintPure)
	float GetHealth();

	UFUNCTION(BlueprintCallable, Category = "Player Stored Data", BlueprintPure)
	float GetMaxHealth();

	UFUNCTION(BlueprintCallable, Category = "Player Stored Data")
	void SetHealth(float NewHeath);

	UFUNCTION(BlueprintCallable, Category = "Player Stored Data")
	void SetMaxHealth(float NewMaxHeath);

	UFUNCTION(BlueprintCallable, Category = "Player Stored Data", BlueprintPure)
	EPlayerStatus GetPlayerStatus();

	UFUNCTION(BlueprintCallable, Category = "Player Stored Data")
	void SetPlayerStatus(EPlayerStatus NewStatus);

	UFUNCTION(BlueprintCallable, Category = "Player Stored Data")
	void CalculateAmmo(EWeaponType WeaponType, int Value);

	UFUNCTION(BlueprintCallable, Category = "Player Stored Data", BlueprintPure)
	int GetTotalAmmo(EWeaponType WeaponType);

	UFUNCTION(BlueprintCallable, Category = "Player Stored Data")
	void SetTotalAmmo(EWeaponType WeaponType, int Value, const bool bAdd = true);

};
