// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_AttackPlayer.h"
#include "ZombieSurvival/Interface/IZSEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "ZS_AIController.h"
#include "AIController.h"

UBTTask_AttackPlayer::UBTTask_AttackPlayer(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Attacking");
}

EBTNodeResult::Type UBTTask_AttackPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	bool bOutOfRange = !OwnerComp.GetBlackboardComponent()->GetValueAsBool(GetSelectedBlackboardKey());
	if (bOutOfRange)
	{
		// FinishWithSuccess
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	if (auto* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		auto Enemy = OwnerComp.GetAIOwner()->GetPawn();		
		if (Enemy->GetClass()->ImplementsInterface(UIZSEnemy::StaticClass()))
		{
			// Get Player
			if (auto* Target = OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"))
			{
				if (auto* PlayerTarget = Cast<ACharacter>(Target))
				{
					Player = PlayerTarget;
				}
			}
			if (IIZSEnemy::Execute_DoesCurrentMontageFinish(Enemy))
			{
				// Stop
				OwnerComp.GetAIOwner()->StopMovement();
				// Then Attack
				IIZSEnemy::Execute_SetEnemyStateInterface(Enemy, EEnemyState::Attack);
				// FinishWithSuccess
				OwnerComp.GetBlackboardComponent()->SetValueAsFloat("AttackTime", IIZSEnemy::Execute_GetCurrentAttackAnimationDuration(Enemy));
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}

