// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyAbilityBase.h"
#include "EnemyStunAbility.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Class), meta = (BlueprintSpawnableComponent), Blueprintable, Category = "Enemy Abilities")
class ZOMBIESURVIVAL_API UEnemyStunAbility : public UEnemyAbilityBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	class UAnimMontage* StunAttackAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	class UNiagaraSystem* StunAttackFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	float StunRadius = 300;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	float StunTime = 0.8f;

	virtual void ActiveAbility() override;

	virtual void EndAbility() override;
};
