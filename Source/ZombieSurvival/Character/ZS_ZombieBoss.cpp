// Fill out your copyright notice in the Description page of Project Settings.


#include "ZS_ZombieBoss.h"
#include "Ability/EnemyMelleAbility.h"
#include "Ability/EnemyRangeAbility.h"
#include "Ability/EnemyStunAbility.h"

AZS_ZombieBoss::AZS_ZombieBoss()
{
	MeleeAbilityComponent = CreateDefaultSubobject<UEnemyMelleAbility>("MeleeAbilityComponent");
	RangeAbilityComponent = CreateDefaultSubobject<UEnemyRangeAbility>("RangeAbilityComponent");
	StunAbilityComponent = CreateDefaultSubobject<UEnemyStunAbility>("StunAbilityComponent");
}
