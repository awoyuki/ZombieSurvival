// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ZombieSurvival/PoolingSystem/Poolable.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "BulletBase.generated.h"

UCLASS(Blueprintable)
class ZOMBIESURVIVAL_API ABulletBase : public AActor, public IPoolable
{
	GENERATED_BODY()
	
protected:

	float BulletDamage;

public:	
	// Sets default values for this actor's properties
	ABulletBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bullet Data")
	UStaticMeshComponent* BulletMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bullet Data")
	USphereComponent* TriggerSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bullet Data")
	UProjectileMovementComponent* ProjectileComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bullet Data")
	float BulletSpeed = 800.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bullet Data")
	FVector BulletVeclocity = FVector(2000.0f,0,0);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bullet Data")
	class UParticleSystem* ImpactFX;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bullet Data")
	class USoundCue* ImpactSFX;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBulletHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

public:	

	virtual void UpdateBulletVelocity(FVector NewVelocity);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void StartMoving(FVector Target, float Damage);

	virtual void EndMove(const FHitResult& Hit);
};
