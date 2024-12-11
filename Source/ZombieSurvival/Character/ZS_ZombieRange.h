// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZS_ZombieBase.h"
#include "ZS_ZombieRange.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESURVIVAL_API AZS_ZombieRange : public AZS_ZombieBase
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability")
	class UEnemyRangeAbility* RangeAbilityComponent;


	AZS_ZombieRange();
};
