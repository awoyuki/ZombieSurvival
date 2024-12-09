// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "ZombieSurvival/PoolingSystem/Poolable.h"
#include "BulletBase.generated.h"

UCLASS()
class ZOMBIESURVIVAL_API ABulletBase : public AActor, public IPoolable
{
	GENERATED_BODY()
	
private:
	FVector TargetLocation;
	float StartHeight;
	float TargetDistance;

public:	
	// Sets default values for this actor's properties
	ABulletBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* BulletMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BulletSpeed = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UParticleSystem* ImpactFX;

	FTimerHandle MoveTimeHandle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void StartMoving(FVector Target);

	void Moving();

	void EndMove();
};
