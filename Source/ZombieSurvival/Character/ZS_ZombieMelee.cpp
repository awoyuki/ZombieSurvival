// Fill out your copyright notice in the Description page of Project Settings.


#include "ZS_ZombieMelee.h"
#include "Ability/EnemyMelleAbility.h"

AZS_ZombieMelee::AZS_ZombieMelee()
{
	MeleeAbilityComponent = CreateDefaultSubobject<UEnemyMelleAbility>("MeleeAbilityComponent");
}
