// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FindPlayerLocation.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESURVIVAL_API UBTTask_FindPlayerLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	explicit UBTTask_FindPlayerLocation(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy AI")
	bool bSearchRandom = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy AI")
	float SearchRadius = 950.0f;
};
