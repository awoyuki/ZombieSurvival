// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BulletBase.h"
#include "GrenadeBullet.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ZOMBIESURVIVAL_API AGrenadeBullet : public ABulletBase
{
	GENERATED_BODY()



protected:

	FVector TargetLocation;
	float StartHeight;
	float TargetDistance;

	FTimerHandle MoveTimeHandle;

public:

	AGrenadeBullet();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BulletRadius = 150;

	FVector CalculateProjectileVelocity(FVector StartLocation, FVector Target);

	virtual void StartMoving(FVector Target, float Damage) override;

	virtual void EndMove(const FHitResult& Hit) override;
};
