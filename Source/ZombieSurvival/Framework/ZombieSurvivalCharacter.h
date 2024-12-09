// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "ZombieSurvivalCharacter.generated.h"

UCLASS(Blueprintable)
class AZombieSurvivalCharacter : public ACharacter
{
	GENERATED_BODY()

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

	/** Mouse Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InputChangeWeaponAction;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Property)
	double AngularDistanceBetweenPlayerAndCursor();

	FVector GetMouseLocation();

protected:

	APlayerController* PlayerController;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	float PlayAnimMontage(class UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName);

	virtual void OnPlayerMove(const FInputActionValue& value);

	virtual void OnPlayerMouseStart();

	virtual void OnPlayerMouseEnd();

	virtual void OnPlayerChangeWeapon();

	virtual void FixPlayerRotation();

};

