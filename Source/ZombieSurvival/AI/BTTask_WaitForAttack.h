// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_WaitBlackboardTime.h"
#include "BTTask_WaitForAttack.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESURVIVAL_API UBTTask_WaitForAttack : public UBTTask_WaitBlackboardTime
{
	GENERATED_BODY()

public:
	explicit UBTTask_WaitForAttack(FObjectInitializer const& ObjectInitializer);
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
