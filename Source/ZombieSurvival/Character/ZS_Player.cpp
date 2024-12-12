#include "ZS_Player.h"
#include <Sound/SoundCue.h>
#include <Kismet/GameplayStatics.h>
#include "ZombieSurvival/Item/WeaponBase.h"
#include "ZombieSurvival/PoolingSystem/PoolSubsystem.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
AZS_Player::AZS_Player()
{
	// Set this character to call Tick() every frame. You can turn this off to improve performance if you don't need it.

	StunVFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MuzzleVFX"));
	StunVFX->SetupAttachment(RootComponent);
	StunVFX->SetAutoActivate(false);

	PrimaryActorTick.bCanEverTick = true;
	this->Tags.Add(TEXT("Player"));

}

// Called when the game starts or when spawned
void AZS_Player::BeginPlay()
{
	Super::BeginPlay();

	UPoolSubsystem* PoolSubsystem = GetWorld()->GetSubsystem<UPoolSubsystem>();

	//GetDefaultWeapon
	if (DefaultWeaponData == nullptr)
		return;

	PickUpWeapon_Implementation(DefaultWeaponData);
}

// Called every frame
void AZS_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AZS_Player::OnPlayerMouseStart()
{
	if (ZSPlayerState->GetPlayerStatus() == EPlayerStatus::Default)
	{
		if (!IsValid(CurrentWeapon))  return;

		if (CurrentWeapon->WeaponData->WeaponType == EWeaponType::AssaultRifle)
			CurrentWeapon->WeaponFire();
		else
			CurrentWeapon->WeaponCharge();
	}
}

void AZS_Player::OnPlayerMouseEnd()
{
	if (!IsValid(CurrentWeapon)) return;

	if (CurrentWeapon->WeaponData->WeaponType == EWeaponType::AssaultRifle)
		CurrentWeapon->WeaponEndFire();
	else 
	{
		CurrentWeapon->WeaponFireCharge();
		HideCursorVFX();
	}
}

void AZS_Player::OnPlayerChangeWeapon()
{
	if (Weapons.Num() <= 1)
		return;
	if (ZSPlayerState->GetPlayerStatus() == EPlayerStatus::Default)
	{
		int NewWeaponIndex = Weapons.FindLast(CurrentWeapon);
		NewWeaponIndex = (NewWeaponIndex + 1) % Weapons.Num();
		CurrentWeapon->OnStoredWeapon();
		CurrentWeapon = Weapons[NewWeaponIndex];
		CurrentWeapon->OnSwitchWeapon();
		HideCursorVFX();
	}
}

void AZS_Player::OnPlayerDead()
{
	Super::OnPlayerDead();
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetActorLocation());
	GetMesh()->SetSimulatePhysics(true);
	OnPlayerMouseEnd();
	auto ZGameState = UGameplayStatics::GetGameState(GetWorld());
	if (ZGameState->GetClass()->ImplementsInterface(UIZSGameState::StaticClass()))
	{
		IIZSGameState::Execute_SetGameStatus(ZGameState, EGameStatus::Lose);
	}
}

void AZS_Player::OnPlayerReload()
{
	if (CurrentWeapon == nullptr)
		return;
	if (ZSPlayerState->GetPlayerStatus() == EPlayerStatus::Default)
	{
		CurrentWeapon->ManualWeaponReload();
		HideCursorVFX();
	}
}

void AZS_Player::Stunned()
{
	StunVFX->DeactivateSystem();
	GetWorldTimerManager().ClearTimer(StunHandle);
	ZSPlayerState->SetPlayerStatus(EPlayerStatus::Default);
}

void AZS_Player::OnPlayerStunned(float StunTime)
{
	StunVFX->ActivateSystem();
	ZSPlayerState->SetPlayerStatus(EPlayerStatus::Stunned);
	GetWorldTimerManager().ClearTimer(StunHandle);
	GetWorldTimerManager().SetTimer(StunHandle, this, &AZS_Player::Stunned, 0.1f, true, StunTime);
}

void AZS_Player::OnPlayerInteractWithWeapon(UWeaponData* WeaponData, EWeaponState State)
{
	switch (State)
	{
	case EWeaponState::Firing:
		PlayAnimMontage(WeaponData->FireAnimation, 1, NAME_None);
		break;
	case EWeaponState::Reloading:
		PlayAnimMontage(WeaponData->ReloadAnimation, 1, NAME_None);
		break;
	case EWeaponState::Storing:
		break;
	case EWeaponState::Holding:
		break;
	default:
		break;
	}
}

void AZS_Player::UpdateMovementSpeed(float NewSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
}

void AZS_Player::ShowCursorVFX()
{
	FXCursorComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, FVector::ZeroVector, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), false, true, ENCPoolMethod::None, true);
	GetWorldTimerManager().SetTimer(CursorFXHandle, this, &AZS_Player::UpdateCursorVFXLocation, 0.02f, true, 0);
}

void AZS_Player::UpdateCursorVFXLocation()
{
	if(FXCursorComponent != nullptr)
		FXCursorComponent->SetWorldLocation(GetMouseLocation());
}


void AZS_Player::HideCursorVFX()
{
	GetWorldTimerManager().ClearTimer(CursorFXHandle);
	if (FXCursorComponent != nullptr)
	{
		FXCursorComponent->DeactivateImmediate();
	}
}

void AZS_Player::PickupKey()
{
	Keys++;
}

int AZS_Player::UseKey()
{
	int keyUsed = Keys;
	Keys = 0;
	return keyUsed;
}

void AZS_Player::Healing(float Value)
{
	ZSPlayerState->SetHealth(ZSPlayerState->GetHealth() + Value);
}

void AZS_Player::AddTotalAmmo(EWeaponType TypeWeapon, float Value)
{
	ZSPlayerState->SetTotalAmmo(TypeWeapon, Value);
}

int AZS_Player::GetCurrentWeaponTotalAmmo()
{
	if(CurrentWeapon == nullptr)
		return 0;

	return ZSPlayerState->GetTotalAmmo(CurrentWeapon->WeaponData->WeaponType);
}

UWeaponData* AZS_Player::GetCurrentWeaponData()
{
	if (CurrentWeapon == nullptr)
		return nullptr;
	return CurrentWeapon->WeaponData;
}

int AZS_Player::GetCurrentWeaponAmmo()
{
	if (CurrentWeapon == nullptr)
		return 0;

	return CurrentWeapon->CurrentAmmo;
}

AWeaponBase* AZS_Player::GetCurrentWeapon_Implementation()
{
	if (CurrentWeapon == nullptr)
		return nullptr;

	return CurrentWeapon;
}

void AZS_Player::PickUpWeapon_Implementation(UWeaponData* NewWeaponData)
{
	AWeaponBase* NewWeapon = GetWorld()->GetSubsystem<UPoolSubsystem>()->SpawnFromPool<AWeaponBase>(AWeaponBase::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
	if (IsValid(NewWeapon)) {
		NewWeapon->OnEquippedWeapon(this, NewWeaponData);
	}
	NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
	NewWeapon->SetOwner(this);
	Weapons.Add(NewWeapon);
	if (IsValid(CurrentWeapon))
		CurrentWeapon->OnStoredWeapon();
	CurrentWeapon = NewWeapon;
	HideCursorVFX();
}

float AZS_Player::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Health = ZSPlayerState->GetHealth();
	if (Health > 0)
	{
		ZSPlayerState->SetHealth(FMath::Clamp(Health - DamageAmount, 0, ZSPlayerState->GetMaxHealth()));
		if(ZSPlayerState->GetHealth() <= 0)
			OnPlayerDead();
		else
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, GetActorLocation());
	}
	return DamageAmount;
}

