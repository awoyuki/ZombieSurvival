// Fill out your copyright notice in the Description page of Project Settings.


#include "PoolSubsystem.h"
#include "Poolable.h"


void UPoolSubsystem::SpawnFromPool(TSubclassOf<AActor> PoolClass, FVector Location, FRotator Rotation, AActor*& SpawnedActor)
{
	SpawnedActor = SpawnFromPool<AActor>(PoolClass, Location, Rotation);
}


void UPoolSubsystem::ReturnToPool(AActor* Poolable)
{
	UClass* PoolableClass = Poolable->GetClass();

	if (PoolableClass->ImplementsInterface(UPoolable::StaticClass()))
	{
		IPoolable::Execute_OnReturnToPool(Poolable);
		FPoolArray* ObjectPool = ObjectPools.Find(PoolableClass);
		if (ObjectPool == nullptr)
		{
			ObjectPools.Add(PoolableClass);
			ObjectPool = ObjectPools.Find(PoolableClass);
		}
		ObjectPool->Add(Poolable);
		Poolable->SetActorEnableCollision(false);
		Poolable->SetActorHiddenInGame(true);
	}
	else 
	{
		Poolable->Destroy();
	}
}
