// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "ZS_AIController.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESURVIVAL_API AZS_AIController : public AAIController
{
	GENERATED_BODY()
	
public:

	explicit AZS_AIController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:

	FTimerHandle InitTimeHandle;

	virtual void OnPossess(APawn* InPawn) override;

private:

	class UAISenseConfig_Sight* SightConfig;

	void SetupBlackBoard();

	void SetupPerceptionSystem();

	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus);

	void ResetPerceptionSystem();
};
