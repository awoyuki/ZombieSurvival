#pragma once

#include "CoreMinimal.h"
#include "ZombieSurvival/Item/WeaponBase.h"
#include "ZombieSurvival/Framework/ZombieSurvivalCharacter.h"
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



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void PickupWeapon(AWeaponBase* NewWeapon);
	
	virtual void OnPlayerMouseStart() override;

	virtual void OnPlayerMouseEnd() override;

	virtual void OnPlayerFiringWeapon(FWeaponDataStruct WeaponData) override;

};
