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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Common Data")
	class UParticleSystem* BulletDefaultImpact;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Common Data")
	class UParticleSystem* BulletBodyImpact;
};
