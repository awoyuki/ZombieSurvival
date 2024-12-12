// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "ZombieSurvival/Framework/ZombieSurvivalGameState.h"
#include "ZombieSurvival/Framework/ZombieSurvivalPlayerState.h"
#include "ZombieSurvival/Interface/IZSCharacter.h"
#include "ZombieSurvivalCharacter.generated.h"

UCLASS(Blueprintable)
class AZombieSurvivalCharacter : public ACharacter, public IIZSCharacter
{
	GENERATED_BODY()

private:
	double CurrentAngular;

public:
	AZombieSurvivalCharacter();

	virtual void BeginPlay() override;

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InputMoveAction;

	/** Mouse Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InputMouseAction;

	/** Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InputChangeWeaponAction;

	/** Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InputReloadAction;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Property)
	double AngularDistanceBetweenPlayerAndCursor();

	void UpdateAngularDistanceBetweenPlayerAndCursor();	

	FVector GetMouseLocation();

	// Cached Game Variables
	AZombieSurvivalPlayerState* ZSPlayerState;


protected:

	APlayerController* PlayerController;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	float PlayAnimMontage(class UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName);

	virtual void OnPlayerMove(const FInputActionValue& value);

	virtual void OnPlayerMouseStart();

	virtual void OnPlayerMouseEnd();

	virtual void OnPlayerChangeWeapon();

	virtual void OnPlayerReload();

	virtual void OnPlayerDead();

	virtual void FixPlayerRotation();

	virtual bool IsDead_Implementation() override;

	class UAIPerceptionStimuliSourceComponent* StimulusSource;

	void SetupStimulusSource();

};

