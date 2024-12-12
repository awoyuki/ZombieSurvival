// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_WaitForAttack.h"
#include "ZombieSurvival/Character/ZS_ZombieBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "ZS_AIController.h"


UBTTask_WaitForAttack::UBTTask_WaitForAttack(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Wait For Attack";	
}

void UBTTask_WaitForAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	auto Enemy = OwnerComp.GetAIOwner()->GetPawn();
	if (Enemy->GetClass()->ImplementsInterface(UIZSEnemy::StaticClass()))
	{
		IIZSEnemy::Execute_SetEnemyStateInterface(Enemy, EEnemyState::Idle);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool("InAttackRange", false);
	}
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}