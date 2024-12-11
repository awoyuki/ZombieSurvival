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

	AZS_AIController* EnemyController = Cast<AZS_AIController>(OwnerComp.GetAIOwner());
	if (auto* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		// EnemyBehaviour
		if (AZS_ZombieBase* Enemy = Cast<AZS_ZombieBase>(OwnerComp.GetAIOwner()->GetPawn()))
		{
			Enemy->SetEnemyState(EEnemyState::Chasing);
			// Get Player
			if (auto* Target = OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"))
			{
				if (auto* PlayerTarget = Cast<ACharacter>(Target))
				{
					Player = PlayerTarget;
				}
			}
			if (auto* ZSPlayer = Cast<AZombieSurvivalCharacter>(Player)) 
			{
				EnemyController->GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), Enemy->GetDistanceTo(Player) <= Enemy->CurrentAttackRange &&
					EnemyController->GetBlackboardComponent()->GetValueAsBool("CanSeePlayer") && !ZSPlayer->IsDead()
				);
			}				
		}
	}
}
