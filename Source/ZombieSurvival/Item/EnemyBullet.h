// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BulletBase.h"
#include "EnemyBullet.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ZOMBIESURVIVAL_API AEnemyBullet : public ABulletBase
{
	GENERATED_BODY()

public:

	virtual void EndMove(const FHitResult& Hit) override;
};
