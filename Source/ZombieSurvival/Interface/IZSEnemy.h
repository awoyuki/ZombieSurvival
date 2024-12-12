// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IZSEnemy.generated.h"


UENUM(BlueprintType, Blueprintable)
enum class EEnemyState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Patrol UMETA(DisplayName = "Patrol"),
	Chasing UMETA(DisplayName = "Chasing"),
	Attack UMETA(DisplayName = "Attack"),
	Death UMETA(DisplayName = "Death"),
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIZSEnemy : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ZOMBIESURVIVAL_API IIZSEnemy
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Enemy Interface")
	bool IsDead();
	virtual bool IsDead_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Enemy Interface")
	float GetCurrentAttackRange();
	virtual float GetCurrentAttackRange_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Enemy Interface")
	EEnemyState GetEnemyState();
	virtual EEnemyState GetEnemyState_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Enemy Interface")
	void SetEnemyStateInterface(EEnemyState newState);
	virtual void SetEnemyStateInterface_Implementation(EEnemyState newState);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Enemy Interface")
	bool DoesCurrentMontageFinish();
	virtual bool DoesCurrentMontageFinish_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Enemy Interface")
	float GetCurrentAttackAnimationDuration();
	virtual float GetCurrentAttackAnimationDuration_Implementation();

};
