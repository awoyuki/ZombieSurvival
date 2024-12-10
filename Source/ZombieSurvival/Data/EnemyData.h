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
	Melee UMETA(DisplayName = "Melee"),
	Range UMETA(DisplayName = "Range"),
	Boss UMETA(DisplayName = "Boss")
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
	USkeletalMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Data")
	EEnemyType EnemyType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Data")
	float Health = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Data")
	float AttackRange = 150;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Data")
	float AttackSpeed = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Data")
	float BaseDamage = 25;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Data")
	float MovementSpeed = 300;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Data")
	class UAnimMontage* AttackAnimation;
};
