// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZS_ZombieBase.h"
#include "ZS_ZombieBoss.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESURVIVAL_API AZS_ZombieBoss : public AZS_ZombieBase
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability")
	class UEnemyMelleAbility* MeleeAbilityComponent;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability")
	class UEnemyRangeAbility* RangeAbilityComponent;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability")
	class UEnemyStunAbility* StunAbilityComponent;


	AZS_ZombieBoss();
};
