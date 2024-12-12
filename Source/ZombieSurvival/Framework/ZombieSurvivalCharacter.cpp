// Copyright Epic Games, Inc. All Rights Reserved.

#include "Templates/SubclassOf.h"
#include "ZombieSurvivalCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "ZombieSurvivalPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AZombieSurvivalCharacter::AZombieSurvivalCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, -45.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// Setup For AI
	SetupStimulusSource();

}

void AZombieSurvivalCharacter::BeginPlay()
{
	Super::BeginPlay();
	ZSPlayerState = Cast<AZombieSurvivalPlayerState>(GetPlayerState());
	PlayerController = Cast<APlayerController>(Controller);
	//Add Input Mapping Context
	if (IsValid(PlayerController))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	
}

void AZombieSurvivalCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Setup mouse input events
		EnhancedInputComponent->BindAction(InputMoveAction, ETriggerEvent::Triggered, this, &AZombieSurvivalCharacter::OnPlayerMove);
		EnhancedInputComponent->BindAction(InputChangeWeaponAction, ETriggerEvent::Started, this, &AZombieSurvivalCharacter::OnPlayerChangeWeapon);
		EnhancedInputComponent->BindAction(InputReloadAction, ETriggerEvent::Started, this, &AZombieSurvivalCharacter::OnPlayerReload);
		EnhancedInputComponent->BindAction(InputMouseAction, ETriggerEvent::Started, this, &AZombieSurvivalCharacter::OnPlayerMouseStart);
		EnhancedInputComponent->BindAction(InputMouseAction, ETriggerEvent::Completed, this, &AZombieSurvivalCharacter::OnPlayerMouseEnd);
		EnhancedInputComponent->BindAction(InputMouseAction, ETriggerEvent::Canceled, this, &AZombieSurvivalCharacter::OnPlayerMouseEnd);
	}
}



void AZombieSurvivalCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
	if (ZSPlayerState)
	{
		if (ZSPlayerState->GetPlayerStatus() == EPlayerStatus::Stunned) return;
		UpdateAngularDistanceBetweenPlayerAndCursor();
		FixPlayerRotation();
	}
}


float AZombieSurvivalCharacter::PlayAnimMontage(class UAnimMontage* AnimMontage, float InPlayRate = 0, FName StartSectionName = NAME_None)
{
	UAnimInstance* AnimInstance = (GetMesh()) ? GetMesh()->GetAnimInstance() : nullptr;
	if (AnimMontage && AnimInstance)
	{
		float const Duration = AnimInstance->Montage_Play(AnimMontage, InPlayRate);
		if (Duration > 0.f)
		{
			// Start at a given Section.
			if (StartSectionName != NAME_None)
			{
				AnimInstance->Montage_JumpToSection(StartSectionName, AnimMontage);
			}
			return Duration;
		}
	}
	return 0.f;
}


void AZombieSurvivalCharacter::OnPlayerMove(const FInputActionValue& value)
{
	if (ZSPlayerState->GetPlayerStatus() == EPlayerStatus::Stunned) return;

	FVector2D InputVector = value.Get<FVector2D>();

	const FRotator Rotation = GetControlRotation();
	const float Offset = GetCameraBoom()->GetRelativeRotation().Yaw;
	const FRotator YawRotation(0, Rotation.Yaw + Offset, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, InputVector.X, false);
	AddMovementInput(RightDirection, InputVector.Y, false);
}

void AZombieSurvivalCharacter::OnPlayerMouseStart()
{
	if (ZSPlayerState->GetPlayerStatus() == EPlayerStatus::Dead) return;
}

void AZombieSurvivalCharacter::OnPlayerMouseEnd()
{
	if (ZSPlayerState->GetPlayerStatus() == EPlayerStatus::Dead) return;
}

void AZombieSurvivalCharacter::OnPlayerChangeWeapon()
{
	if (ZSPlayerState->GetPlayerStatus() == EPlayerStatus::Dead) return;
}

void AZombieSurvivalCharacter::OnPlayerReload()
{
	if (ZSPlayerState->GetPlayerStatus() == EPlayerStatus::Dead) return;
}

void AZombieSurvivalCharacter::OnPlayerDead()
{
	ZSPlayerState->SetPlayerStatus(EPlayerStatus::Dead);
	if (StimulusSource)
	{
		StimulusSource->UnregisterFromSense(TSubclassOf<UAISense_Sight>());
		StimulusSource->UnregisterFromPerceptionSystem();
	}
}

void AZombieSurvivalCharacter::FixPlayerRotation()
{
	float rotateAngle = AngularDistanceBetweenPlayerAndCursor();
	if (rotateAngle < -90 || rotateAngle > 90)
	{
		const FQuat newQuat = (FRotator(0, rotateAngle - (rotateAngle < -90 ? (-90) : 90), 0)).Quaternion();
		AddActorLocalRotation(newQuat);
	}
}

bool AZombieSurvivalCharacter::IsDead_Implementation()
{
	return ZSPlayerState->GetHealth() <= 0;
}

void AZombieSurvivalCharacter::SetupStimulusSource()
{
	StimulusSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
	if (StimulusSource)
	{
		StimulusSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
		StimulusSource->RegisterWithPerceptionSystem();
	}
}

double AZombieSurvivalCharacter::AngularDistanceBetweenPlayerAndCursor()
{
	return CurrentAngular;
}

void AZombieSurvivalCharacter::UpdateAngularDistanceBetweenPlayerAndCursor()
{
	FVector PawnLocation = GetActorLocation();
	FVector MouseLocation = GetMouseLocation();
	MouseLocation.Z = PawnLocation.Z;

	FVector TargetDirection = MouseLocation - PawnLocation;
	double dotValue = GetActorRightVector().Dot(TargetDirection);

	FRotator TargetRotator = UKismetMathLibrary::MakeRotFromX(TargetDirection);

	// Convert FRotator to FQuat
	FQuat ControllerQuat = GetActorRotation().Quaternion();
	FQuat TargetQuat = TargetRotator.Quaternion();

	double AngleRadians = ControllerQuat.AngularDistance(TargetQuat);
	CurrentAngular = UKismetMathLibrary::RadiansToDegrees(AngleRadians) * (dotValue > 0 ? 1 : -1);
}

FVector AZombieSurvivalCharacter::GetMouseLocation()
{
	// Looking for the mouse location in the world
	FHitResult Hit;
	bool bHitSuccessful = false;

	if(IsValid(PlayerController))
		bHitSuccessful = PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, Hit);
	FVector MousePosition = FVector::ZeroVector;
	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		MousePosition = Hit.Location;
	}

	return MousePosition;
}




