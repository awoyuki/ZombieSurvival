#pragma once

#include "CoreMinimal.h"
#include "ZombieSurvival/Item/WeaponBase.h"
#include "ZombieSurvival/Framework/ZombieSurvivalCharacter.h"
#include "ZombieSurvival/Framework/ZombieSurvivalPlayerState.h"
#include "ZS_Player.generated.h"


UCLASS(Blueprintable)
class ZOMBIESURVIVAL_API AZS_Player : public AZombieSurvivalCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZS_Player();


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FName WeaponSocket = TEXT("weapon_r_socket");

	// Current Weapon in use
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AWeaponBase* CurrentWeapon;

	// List of weapon
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<AWeaponBase*> Weapons;


	// Cached Game Variables
	AZombieSurvivalGameState* ZSGameState;
	AZombieSurvivalPlayerState* ZSPlayerState;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void PickupWeapon(AWeaponBase* NewWeapon);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	virtual void OnPlayerMouseStart() override;

	virtual void OnPlayerMouseEnd() override;

	virtual void OnPlayerChangeWeapon() override;

	virtual void OnPlayerInteractWithWeapon(UWeaponData* WeaponData, EWeaponState State);

};
