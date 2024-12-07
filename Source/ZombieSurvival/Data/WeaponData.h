// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Animation/AnimMontage.h"
#include "WeaponData.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class WeaponType : uint8
{
	AssaultRifle UMETA(DisplayName = "Assault Rifle"),
	BurstRifle UMETA(DisplayName = "Burst Rifle"),
	GrenadeLauncher UMETA(DisplayName = "Grenade Launcher")
};


USTRUCT(BlueprintType)
struct FWeaponDataStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Data")
	int32 ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Data")
	FName WeaponName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Data")
	WeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Data")
	class UStaticMesh* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Data")
	class UStaticMesh* WeaponMagMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Data")
	FName MagSocketName = TEXT("mag_socket");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Data")
	int32 AmmoPerMag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Data")
	float FireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Data")
	float ReloadSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Data")
	class UAnimMontage* FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Data")
	class USoundCue* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Data")
	class UAnimMontage* ReloadAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Data")
	class UParticleSystem* BulletMuzzle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Data")
	class UParticleSystem* BulletTrailEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Data")
	class UParticleSystem* BulletImpact;

};

UCLASS()
class ZOMBIESURVIVAL_API UWeaponData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Data")
	TArray<FWeaponDataStruct> WeaponDataArray;
};
