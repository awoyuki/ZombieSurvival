#include "ZS_Player.h"

// Sets default values
AZS_Player::AZS_Player()
{
	// Set this character to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	this->Tags.Add(TEXT("Player"));

}

// Called when the game starts or when spawned
void AZS_Player::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AZS_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AZS_Player::OnPlayerMouseStart()
{
	if (!IsValid(CurrentWeapon)) return;

	if(CurrentWeapon->WeaponData.WeaponType == EWeaponType::AssaultRifle)
		CurrentWeapon->WeaponFire();

}

void AZS_Player::OnPlayerMouseEnd()
{
	if (!IsValid(CurrentWeapon)) return;

	if (CurrentWeapon->WeaponData.WeaponType == EWeaponType::AssaultRifle)
		CurrentWeapon->WeaponEndFire();
}

void AZS_Player::OnPlayerInteractWithWeapon(FWeaponDataStruct WeaponData, EWeaponState State)
{
	switch (State)
	{
	case EWeaponState::Firing:
		PlayAnimMontage(WeaponData.FireAnimation, 1, NAME_None);
		break;
	case EWeaponState::Reloading:
		PlayAnimMontage(WeaponData.ReloadAnimation, 1, NAME_None);
		break;
	case EWeaponState::Storing:
		break;
	case EWeaponState::Holding:
		break;
	default:
		break;
	}
}



void AZS_Player::PickupWeapon(AWeaponBase* NewWeapon)
{
	NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
	Weapons.Add(NewWeapon);
	CurrentWeapon = NewWeapon;
	NewWeapon->SetOwner(this);
}

