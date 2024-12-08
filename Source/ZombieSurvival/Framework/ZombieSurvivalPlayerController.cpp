// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "ZombieSurvivalCharacter.h"
#include "ZombieSurvivalPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"

AZombieSurvivalPlayerController::AZombieSurvivalPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AZombieSurvivalPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
		ICharacter = CastChecked<IICharacter>(GetCharacter());
	}

}

void AZombieSurvivalPlayerController::Tick(float DeltaTime)
{
	FixPlayerRotation();
}



void AZombieSurvivalPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events
		EnhancedInputComponent->BindAction(InputMoveAction, ETriggerEvent::Triggered, this, &AZombieSurvivalPlayerController::OnPlayerMove);
		EnhancedInputComponent->BindAction(InputMouseAction, ETriggerEvent::Triggered, this, &AZombieSurvivalPlayerController::OnPlayerMouseTrigger);
		EnhancedInputComponent->BindAction(InputMouseAction, ETriggerEvent::Completed, this, &AZombieSurvivalPlayerController::OnPlayerMouseEnd);
		EnhancedInputComponent->BindAction(InputMouseAction, ETriggerEvent::Canceled, this, &AZombieSurvivalPlayerController::OnPlayerMouseEnd);
	}
}


void AZombieSurvivalPlayerController::OnPlayerMove(const FInputActionValue& value)
{
	FVector2D InputVector = value.Get<FVector2D>();
	APawn* ControlledPawn = GetPawn();

	const FRotator Rotation = GetControlRotation();
	const float Offset = ICharacter ? ICharacter->GetMovementOffset() : 0;
	const FRotator YawRotation(0, Rotation.Yaw + Offset, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (ControlledPawn != nullptr)
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputVector.X, false);
		ControlledPawn->AddMovementInput(RightDirection, InputVector.Y, false);
	}
}

void AZombieSurvivalPlayerController::OnPlayerMouseTrigger()
{
	if (ICharacter == nullptr) return;
	ICharacter->OnPlayerMouseTrigger();
}

void AZombieSurvivalPlayerController::OnPlayerMouseEnd()
{
	if (ICharacter == nullptr) return;
	ICharacter->OnPlayerMouseEnd();
}

void AZombieSurvivalPlayerController::FixPlayerRotation()
{
	float rotateAngle = AngularDistanceBetweenPlayerAndCursor();
	if (rotateAngle < -90 || rotateAngle > 90) 
	{
		APawn* CurrentPawn = GetPawn();
		if (!IsValid(CurrentPawn))
			return;

		const FQuat newQuat = (FRotator(0, rotateAngle - (rotateAngle < -90 ? (-90) : 90), 0)).Quaternion();
		CurrentPawn->AddActorLocalRotation(newQuat);
	}
}

double AZombieSurvivalPlayerController::AngularDistanceBetweenPlayerAndCursor()
{
	APawn* CurrentPawn = GetPawn();
	if (!IsValid(CurrentPawn))
		return 0;

	FVector PawnLocation = CurrentPawn->GetActorLocation();
	FVector MouseLocation = GetMouseLocation();
	MouseLocation.Z = PawnLocation.Z;

	FVector TargetDirection = MouseLocation - PawnLocation;
	double dotValue = CurrentPawn->GetActorRightVector().Dot(TargetDirection);
	 
	FRotator TargetRotator = UKismetMathLibrary::MakeRotFromX(TargetDirection);

	// Convert FRotator to FQuat
	FQuat ControllerQuat = CurrentPawn->GetActorRotation().Quaternion();
	FQuat TargetQuat = TargetRotator.Quaternion();

	double AngleRadians = ControllerQuat.AngularDistance(TargetQuat);
	return UKismetMathLibrary::RadiansToDegrees(AngleRadians) * (dotValue > 0 ? 1 : -1);

}

FVector AZombieSurvivalPlayerController::GetMouseLocation()
{
	// Looking for the mouse location in the world
	FHitResult Hit;
	bool bHitSuccessful = false;
	bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	FVector MousePosition = FVector::ZeroVector;
	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		MousePosition = Hit.Location;
	}

	return MousePosition;
}
