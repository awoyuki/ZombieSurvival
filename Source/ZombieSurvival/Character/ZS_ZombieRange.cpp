// Fill out your copyright notice in the Description page of Project Settings.


#include "ZS_ZombieRange.h"
#include "Ability/EnemyRangeAbility.h"

AZS_ZombieRange::AZS_ZombieRange()
{
	RangeAbilityComponent = CreateDefaultSubobject<UEnemyRangeAbility>("RangeAbilityComponent");
}
