// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyAbilityBase.h"
#include "EnemyMelleAbility.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Class), meta = (BlueprintSpawnableComponent), Blueprintable, Category="Enemy Abilities")
class ZOMBIESURVIVAL_API UEnemyMelleAbility : public UEnemyAbilityBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	class UAnimMontage* MelleAttackAnimation;

	virtual void ActiveAbility() override;

	virtual void EndAbility() override;

	FTimerHandle RotateTimeHandle;
	virtual void RotateToTarget();
};
