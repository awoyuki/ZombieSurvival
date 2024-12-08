#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "ZombieSurvival/Interface/IWeapon.h"
#include "WeaponBase.generated.h"

UCLASS()
class ZOMBIESURVIVAL_API AWeaponBase : public AActor, public IIWeapon
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AWeaponBase();

	// Weapon Components
	UStaticMeshComponent* WeaponMeshComponent;
	UStaticMeshComponent* MagMeshComponent;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnEquippedWeapon(AZS_Player* NewOwner) override;
};
