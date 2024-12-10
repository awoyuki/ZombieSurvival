// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZombieSurvival/Character/ZS_ZombieBase.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_AttackPlayer.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESURVIVAL_API UBTTask_AttackPlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	explicit UBTTask_AttackPlayer(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	bool DoesMontageFinish(AZS_ZombieBase* Enemy);


};
