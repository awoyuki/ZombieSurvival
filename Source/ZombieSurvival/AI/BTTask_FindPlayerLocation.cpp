// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieSurvival/Character/ZS_ZombieBase.h"
#include "BTTask_FindPlayerLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "AIController.h"

class AIController;

UBTTask_FindPlayerLocation::UBTTask_FindPlayerLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Find Player Location In NavMesh");
}

EBTNodeResult::Type UBTTask_FindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)) 
	{
		// Get Player
		auto PlayerLocation = Player->GetActorLocation();
		if (auto* Target = OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"))
		{
			if (auto* PlayerTarget = Cast<AActor>(Target)) 
			{
				PlayerLocation = PlayerTarget->GetActorLocation();
			}
		}
		// Can't sense target ? -> find by yourself
		if (bSearchRandom)
		{
			FNavLocation Loc;
			if (auto* NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
			{
				// Try to get a random location near player
				if (NavSys->GetRandomPointInNavigableRadius(PlayerLocation, SearchRadius, Loc))
				{
					// EnemyBehaviour
					if (auto* Enemy = Cast<AZS_ZombieBase>(OwnerComp.GetAIOwner()->GetPawn()))
					{
						Enemy->SetEnemyState(EEnemyState::Chasing);
					}
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Loc.Location);
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
					return EBTNodeResult::Succeeded;
				}
			}
		}
		else
		{
			// EnemyBehaviour
			auto Enemy = OwnerComp.GetAIOwner()->GetPawn();
			if (Enemy->GetClass()->ImplementsInterface(UIZSEnemy::StaticClass()))
			{
				IIZSEnemy::Execute_SetEnemyStateInterface(Enemy, EEnemyState::Chasing);
			}
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), PlayerLocation);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
