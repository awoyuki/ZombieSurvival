#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "ZombieSurvival/Interface/IWeapon.h"
#include "ZombieSurvival/Data/WeaponData.h"
#include "ZombieSurvival/PoolingSystem/Poolable.h"
#include "WeaponBase.generated.h"

UCLASS()
class ZOMBIESURVIVAL_API AWeaponBase : public AActor, public IIWeapon, public IPoolable
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AWeaponBase();

	FTimerHandle FireTimerHandle;

	// Weapon Components
	UStaticMeshComponent* WeaponMeshComponent;

	UStaticMeshComponent* MagMeshComponent;

	FName MuzzleSocket = TEXT("muzzle_socket");

	UParticleSystemComponent* MuzzleVFX;

	UParticleSystemComponent* BulletTrailVFX;

	// Weapon Stored Data
	FWeaponDataStruct WeaponData;

private:

	void FiringWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnEquippedWeapon(AZS_Player* NewOwner, FWeaponDataStruct NewWeaponData) override;

	void WeaponFire(); 

	void WeaponEndFire();

	virtual void OnReturnToPool_Implementation() override;
};
