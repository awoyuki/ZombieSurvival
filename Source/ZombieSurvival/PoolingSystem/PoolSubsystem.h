// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Poolable.h"
#include "Subsystems/WorldSubsystem.h"
#include "PoolSubsystem.generated.h"

/**
 * 
 */
USTRUCT()
struct FPoolArray {
	GENERATED_BODY()

	UPROPERTY()
	TArray<AActor*> ObjectPool;

	bool IsEmpty() const {
		return ObjectPool.IsEmpty();
	}

	AActor* Pop() {
		return ObjectPool.Pop();
	}

	void Add(AActor* ActorToAdd) {
		ObjectPool.Add(ActorToAdd);
	}
};

UCLASS()
class ZOMBIESURVIVAL_API UPoolSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "Pool Subsytem", meta=(DeterminesOutputType = "PoolClass", DynamicOutputParam = "SpawnedActor"))
	void SpawnFromPool(TSubclassOf<AActor> PoolClass, FVector Location, FRotator Rotation, AActor*& SpawnedActor);

	template<typename T>
	T* SpawnFromPool(TSubclassOf<AActor> PoolClass, FVector Location, FRotator Rotation);

	UFUNCTION(BlueprintCallable, Category = "Pool Subsytem")
	void ReturnToPool(AActor* Poolable);

	UPROPERTY()
	TMap<UClass*, FPoolArray> ObjectPools;
};


template<typename T>
inline T* UPoolSubsystem::SpawnFromPool(TSubclassOf<AActor> PoolClass, FVector Location, FRotator Rotation)
{
	T* PooledActor = nullptr;

	if (PoolClass.Get()->ImplementsInterface(UPoolable::StaticClass()))
	{
		FPoolArray& ObjectPool = ObjectPools.FindOrAdd(PoolClass);

		if (ObjectPool.IsEmpty())
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			PooledActor = GetWorld()->SpawnActor<T>(PoolClass, Location, Rotation, SpawnParams);
		}
		else
		{
			if (IsValid(ObjectPool.Pop()))
			{
				PooledActor = CastChecked<T>(ObjectPool.Pop());
				PooledActor->SetActorLocationAndRotation(Location, Rotation);
				PooledActor->SetActorEnableCollision(true);
				PooledActor->SetActorHiddenInGame(false);
			}
			// If destroyed by KillZ -> Spawn new one;
			else 
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
				PooledActor = GetWorld()->SpawnActor<T>(PoolClass, Location, Rotation, SpawnParams);
			}
		}

		IPoolable::Execute_OnSpawnFromPool(PooledActor);
	}

	return PooledActor;
}

