// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_AttackPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
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
		if (auto* EnemyController = Cast<AZS_AIController>(OwnerComp.GetAIOwner()))
		{
			if (AZS_ZombieBase* Enemy = Cast<AZS_ZombieBase>(OwnerComp.GetAIOwner()->GetPawn()))
			{
				// Get Player
				if (auto* Target = OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"))
				{
					if (auto* PlayerTarget = Cast<ACharacter>(Target))
					{
						Player = PlayerTarget;
					}
				}
				if (DoesMontageFinish(Enemy)) 
				{
					// Stop
					EnemyController->StopMovement();
					// Then Attack
					Enemy->SetEnemyState(EEnemyState::Attack);
					// FinishWithSuccess
					OwnerComp.GetBlackboardComponent()->SetValueAsFloat("AttackTime", Enemy->EnemyData->AttackSpeed);
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
					return EBTNodeResult::Succeeded;
				}
			}
		}
	}
	return EBTNodeResult::Failed;
}



bool UBTTask_AttackPlayer::DoesMontageFinish(AZS_ZombieBase* Enemy)
{
	return Enemy->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(Enemy->EnemyData->AttackAnimation);
}
