// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyAbilityBase.generated.h"


UCLASS(ClassGroup = (Class), meta = (BlueprintSpawnableComponent), Blueprintable, Category = "Enemy Abilities")
class ZOMBIESURVIVAL_API UEnemyAbilityBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyAbilityBase();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	class AZS_ZombieBase* ZSEnemy;

	AActor* TargetActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	float AbilityRange = 150;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	float DamageRate = 1.0f;

	virtual void OnSelectAbility();

	virtual void ActiveAbility();

	virtual void EndAbility();
};
