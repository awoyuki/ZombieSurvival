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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Data")
	int ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Data")
	FName EnemyName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Data")
	USkeletalMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Data")
	float Health = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Data")
	float BaseDamage = 25;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Data")
	float MovementSpeed = 300;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Data")
	USoundWave* HitSound;
};
