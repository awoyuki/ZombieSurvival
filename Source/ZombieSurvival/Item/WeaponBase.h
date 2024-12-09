#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "ZombieSurvival/Interface/IWeapon.h"
#include "ZombieSurvival/Data/WeaponData.h"
#include "ZombieSurvival/PoolingSystem/Poolable.h"
#include "ZombieSurvival/Framework/ZombieSurvivalGameState.h"
#include "WeaponBase.generated.h"

UENUM()
enum class EWeaponState
{
	Firing, // Gun Firing; 
	Reloading, // Gun Reloading; 
	Storing, // Gun is currently eqquided in inventory
	Holding // Gun is being held in player hand
};

UCLASS()
class ZOMBIESURVIVAL_API AWeaponBase : public AActor, public IIWeapon, public IPoolable
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AWeaponBase();


	// Weapon Components
	UStaticMeshComponent* WeaponMeshComponent;

	UStaticMeshComponent* MagMeshComponent;

	FName MuzzleSocket = TEXT("muzzle_socket");

	UParticleSystemComponent* MuzzleVFX;

	// Weapon Stored Data
	FWeaponDataStruct WeaponData;

	// Weapon Status
	FTimerHandle FireTimerHandle;
	FTimerHandle ReloadTimerHandle;
	EWeaponState WeaponState;
	int32 CurrentAmmo;
	int32 TotalAmmo;
	bool isFiringCached;

	// Cached Game Variables
	AZombieSurvivalGameState* ZSGameState;

private:

	void FiringWeapon();

	void ReloadWeapon();

	void PlayerAction();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnEquippedWeapon(AZS_Player* NewOwner, FWeaponDataStruct NewWeaponData) override;

	void WeaponFire(); 

	void WeaponEndFire();

	void StartReloadWeapon();

	void WeaponFireOnLineTrace();

	void WeaponFireEmpty();

	void WeaponFireOnSpawnProjectiles();

	virtual void OnReturnToPool_Implementation() override;
};
