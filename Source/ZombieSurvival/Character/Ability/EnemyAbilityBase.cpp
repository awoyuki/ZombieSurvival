// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAbilityBase.h"
#include "ZombieSurvival/Character/ZS_ZombieBase.h"

// Sets default values for this component's properties
UEnemyAbilityBase::UEnemyAbilityBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UEnemyAbilityBase::BeginPlay()
{
	Super::BeginPlay();
	ZSEnemy = Cast<AZS_ZombieBase>(GetOwner());
	
}


void UEnemyAbilityBase::ActiveAbility()
{
}

void UEnemyAbilityBase::EndAbility()
{
}

void UEnemyAbilityBase::OnSelectAbility()
{
	if (ZSEnemy == nullptr) return;

	ZSEnemy->CurrentDamage = ZSEnemy->EnemyData->BaseDamage * DamageRate;
	ZSEnemy->CurrentAttackRange = AbilityRange;
}

