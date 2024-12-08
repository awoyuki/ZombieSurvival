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

	// Current Weapon in use
	int CurrentWeaponIndex;
	// List of weapon
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<AWeaponBase*> Weapons;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void OnPlayerMouseTrigger() override;

	virtual void OnPlayerMouseEnd() override;

};
