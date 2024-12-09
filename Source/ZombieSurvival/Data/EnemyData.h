// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnemyData.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	AssaultRifle UMETA(DisplayName = "Assault Rifle"),
	BurstRifle UMETA(DisplayName = "Burst Rifle"),
	GrenadeLauncher UMETA(DisplayName = "Grenade Launcher")
};

UCLASS()
class ZOMBIESURVIVAL_API UEnemyData : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Data")
	int ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Data")
	FName EnemyName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Data")
	EEnemyType EnemyType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Data")
	float EnemyHealth = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Data")
	float BaseDamage = 25;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Data")
	float MovementSpeed = 500;
};
