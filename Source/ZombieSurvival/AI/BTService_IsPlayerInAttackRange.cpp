// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_IsPlayerInAttackRange.h"
#include "ZombieSurvival/Framework/ZombieSurvivalCharacter.h"
#include "ZombieSurvival/Character/ZS_ZombieBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "ZS_AIController.h"
#include "AIController.h"

UBTService_IsPlayerInAttackRange::UBTService_IsPlayerInAttackRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("IsPlayerInAttackRange");
}

void UBTService_IsPlayerInAttackRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	if (auto* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		// EnemyBehaviour
		auto Enemy = OwnerComp.GetAIOwner()->GetPawn();
		if (Enemy->GetClass()->ImplementsInterface(UIZSEnemy::StaticClass())) 
		{
			IIZSEnemy::Execute_SetEnemyStateInterface(Enemy, EEnemyState::Chasing);
			// Get Player
			if (auto* Target = OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"))
			{
				if (Target->GetClass()->ImplementsInterface(UIZSCharacter::StaticClass()))
				{
					Player = Cast<ACharacter>(Target);
				}
			}
			if (Player->GetClass()->ImplementsInterface(UIZSCharacter::StaticClass()))
			{
				bool isPlayerDead = IIZSCharacter::Execute_IsDead(Player);
				float CurrentAttackRange = IIZSEnemy::Execute_GetCurrentAttackRange(Enemy);
				OwnerComp.GetAIOwner()->GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), Enemy->GetDistanceTo(Player) <= CurrentAttackRange &&
					OwnerComp.GetAIOwner()->GetBlackboardComponent()->GetValueAsBool("CanSeePlayer") && !isPlayerDead);
			}
		}
	}
}
