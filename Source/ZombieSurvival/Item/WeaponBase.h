#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "ZombieSurvival/Data/WeaponData.h"
#include "ZombieSurvival/PoolingSystem/Poolable.h"
#include "ZombieSurvival/Framework/ZombieSurvivalGameState.h"
#include "ZombieSurvival/Framework/ZombieSurvivalPlayerState.h"
#include "WeaponBase.generated.h"

class AZS_Player;

UENUM()
enum class EWeaponState
{
	Firing, // Gun Firing; 
	Reloading, // Gun Reloading; 
	Storing, // Gun is currently eqquided in inventory
	Holding // Gun is being held in player hand
};

UCLASS()
class ZOMBIESURVIVAL_API AWeaponBase : public AActor, public IPoolable
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AWeaponBase();


	// Weapon Components
	UStaticMeshComponent* WeaponMeshComponent;

	FName MuzzleSocket = TEXT("muzzle_socket");

	UParticleSystemComponent* MuzzleVFX;

	// Weapon Stored Data
	UWeaponData* WeaponData;

	// Weapon Status
	FTimerHandle FireTimerHandle;
	FTimerHandle ReloadTimerHandle;
	EWeaponState WeaponState;
	bool isFiringCached;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int CurrentAmmo;


	// Cached Game Variables
	AZombieSurvivalGameState* ZSGameState;
	AZombieSurvivalPlayerState* ZSPlayerState;
	AZS_Player* ZSPlayer;

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

	virtual void OnEquippedWeapon(AZS_Player* NewOwner, UWeaponData* NewWeaponData);

	virtual void OnSwitchWeapon();

	virtual void OnStoredWeapon();

	void ActiveWeapon(bool bActive);

	void WeaponFire(); 

	void WeaponCharge();

	void WeaponFireCharge();

	void WeaponEndFire();

	void StartReloadWeapon();

	void WeaponFireOnLineTrace();

	void WeaponFireEmpty();

	void WeaponFireOnSpawnProjectiles();
};
