#pragma once

#include "CoreMinimal.h"
#include "ZombieSurvival/Framework/ZombieSurvivalCharacter.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "ZS_Player.generated.h"


class AWeaponBase;
enum class EWeaponState;

UCLASS(Blueprintable)
class ZOMBIESURVIVAL_API AZS_Player : public AZombieSurvivalCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZS_Player();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "APlayerZS")
	UWeaponData* DefaultWeaponData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "APlayerZS")
	UParticleSystemComponent* StunVFX;

	/** FX Class that we will spawn */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "APlayerZS")
	UNiagaraSystem* FXCursor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "APlayerZS")
	UNiagaraComponent* FXCursorComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "APlayerZS")
	FName WeaponSocket = TEXT("weapon_r_socket");

	// Current Weapon in use
	AWeaponBase* CurrentWeapon;

	// List of weapon
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<AWeaponBase*> Weapons;

	FTimerHandle CursorFXHandle;

	FTimerHandle StunHandle;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Stunned();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void PickupWeapon(AWeaponBase* NewWeapon);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	virtual void OnPlayerMouseStart() override;

	virtual void OnPlayerMouseEnd() override;

	virtual void OnPlayerChangeWeapon() override;

	virtual void OnPlayerStunned(float StunTime);

	virtual void OnPlayerInteractWithWeapon(UWeaponData* WeaponData, EWeaponState State);

	virtual void UpdateMovementSpeed(float NewSpeed);

	virtual void ShowCursorVFX();

	virtual void UpdateCursorVFXLocation();

	virtual void HideCursorVFX();

	UFUNCTION(BlueprintCallable)
	void Healing(float Value);

	UFUNCTION(BlueprintCallable)
	void AddTotalAmmo(EWeaponType TypeWeapon, float Value);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetCurrentWeaponTotalAmmo();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UWeaponData* GetCurrentWeaponData();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetCurrentWeaponAmmo();

};
