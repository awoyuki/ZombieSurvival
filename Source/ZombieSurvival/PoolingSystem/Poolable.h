// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Poolable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPoolable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ZOMBIESURVIVAL_API IPoolable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Poolable")
	void OnSpawnFromPool();
	virtual void OnSpawnFromPool_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Poolable")
	void OnReturnToPool();
	virtual void OnReturnToPool_Implementation();

};
