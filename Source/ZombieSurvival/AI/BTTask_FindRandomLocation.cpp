// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieSurvival/Character/ZS_ZombieBase.h"
#include "BTTask_FindRandomLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "ZS_AIController.h"

UBTTask_FindRandomLocation::UBTTask_FindRandomLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Find Random Location In NavMesh");
}

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get AI Controller And Its Enemy
	if (auto* Enemy = Cast<AZS_ZombieBase>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		auto Origin = Enemy->GetActorLocation();

		Enemy->SetEnemyState(EEnemyState::Patrol);

		//Get NavSys
		if (auto* NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
		{
			FNavLocation Loc;
			// Navigate to Random Point
			if (NavSys->GetRandomPointInNavigableRadius(Origin, SearchRadius, Loc))
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Loc.Location);
			}

			//Finish with success
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}

	}
	return EBTNodeResult::Failed;
}
