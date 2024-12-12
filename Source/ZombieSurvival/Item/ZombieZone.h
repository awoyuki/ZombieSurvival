// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZombieSurvival/Character/ZS_ZombieBase.h"
#include "Components/BoxComponent.h"
#include "ZombieZone.generated.h"

UCLASS()
class ZOMBIESURVIVAL_API AZombieZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AZombieZone();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* TriggerBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int MaxNumberOfZombies = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<AZS_ZombieBase>> ZombieClasses;

	UPROPERTY()
	class ALevelController* LevelOwner;

	TArray<AZS_ZombieBase*> ZombieList;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SpawnZombie(ALevelController* NewLevelOwner);

	virtual void KillZombie();
};
