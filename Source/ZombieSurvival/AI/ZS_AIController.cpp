// Fill out your copyright notice in the Description page of Project Settings.


#include "ZS_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "ZombieSurvival/Character/ZS_ZombieBase.h"
#include "ZombieSurvival/Framework/ZombieSurvivalCharacter.h"

AZS_AIController::AZS_AIController(const FObjectInitializer& ObjectInitializer)
{
	SetupPerceptionSystem();
}

void AZS_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	GetWorldTimerManager().SetTimer(InitTimeHandle, this, &AZS_AIController::SetupBlackBoard, 0.1f, false, 0.5f);
}

void AZS_AIController::SetupBlackBoard()
{
	if (AZS_ZombieBase* Enemy = Cast<AZS_ZombieBase>(GetPawn()))
	{
		if (UBehaviorTree* Tree = Enemy->GetBehaviorTree())
		{
			UBlackboardComponent* NewBlackBoard;
			UseBlackboard(Tree->BlackboardAsset, NewBlackBoard);
			Blackboard = NewBlackBoard;
			RunBehaviorTree(Tree);
		}
	}
}

void AZS_AIController::SetupPerceptionSystem()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	if(SightConfig)
	{
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
		SightConfig->SightRadius = 500.0f;
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 25.0f;
		SightConfig->PeripheralVisionAngleDegrees = 90.0f;
		SightConfig->SetMaxAge(5.0f);
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 520.f;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

		GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AZS_AIController::OnTargetDetected);
		GetPerceptionComponent()->ConfigureSense(*SightConfig);
	}
}

void AZS_AIController::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus) 
{
	if (auto* TargetCharacter = Cast<AZombieSurvivalCharacter>(Actor))
	{
		if (!GetBlackboardComponent()) return;

		if (GetBlackboardComponent()->GetValueAsBool("CanSeePlayer")) return;
		GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", Stimulus.WasSuccessfullySensed());
		GetBlackboardComponent()->SetValueAsObject("TargetActor", Actor);
	}
}

void AZS_AIController::ResetPerceptionSystem()
{
	GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", false);
}
