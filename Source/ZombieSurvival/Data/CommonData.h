// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CommonData.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESURVIVAL_API UCommonData : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Common Sound")
	class USoundWave* PickupHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Common Sound")
	class USoundWave* PickupAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Common Sound")
	class USoundWave* PickupKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Common Sound")
	class USoundWave* PickupWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Common Sound")
	class USoundWave* SwitchWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Common Data")
	class UParticleSystem* BulletDefaultImpact;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Common Data")
	class UParticleSystem* BulletBodyImpact;
};
