// Fill out your copyright notice in the Description page of Project Settings.


#include "IZSEnemy.h"

// Add default functionality here for any IIZSEnemy functions that are not pure virtual.

bool IIZSEnemy::IsDead_Implementation()
{
    return false;
}

float IIZSEnemy::GetCurrentAttackRange_Implementation()
{
    return 0.0;
}

EEnemyState IIZSEnemy::GetEnemyState_Implementation()
{
    return EEnemyState();
}

void IIZSEnemy::SetEnemyStateInterface_Implementation(EEnemyState newState)
{
}

bool IIZSEnemy::DoesCurrentMontageFinish_Implementation()
{
    return false;
}

float IIZSEnemy::GetCurrentAttackAnimationDuration_Implementation()
{
    return 0.0f;
}
