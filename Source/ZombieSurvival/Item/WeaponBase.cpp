#include "WeaponBase.h"
#include "ZombieSurvival/Character/ZS_Player.h"
#include "ZombieSurvival/Item/BulletBase.h"
#include "ZombieSurvival/PoolingSystem/PoolSubsystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AWeaponBase::AWeaponBase()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Weapon Component
	WeaponMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMeshComponent"));
	WeaponMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
	SetRootComponent(WeaponMeshComponent);

	MuzzleVFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MuzzleVFX"));
	MuzzleVFX->SetupAttachment(WeaponMeshComponent);
	MuzzleVFX->SetAutoActivate(false);

	// Load from asset
	static ConstructorHelpers::FObjectFinder<USoundWave> PickupWeaponVFXAsset{ TEXT("/Game/ZombieSurvivals/Audio/Common/pickup_weapon.pickup_weapon") };
	if (PickupWeaponVFXAsset.Succeeded()) PickupWeaponVFX = PickupWeaponVFXAsset.Object;

	static ConstructorHelpers::FObjectFinder<USoundWave> SwitchWeaponVFXAsset{ TEXT("/Game/ZombieSurvivals/Audio/Common/switch_weapon.switch_weapon") };
	if (SwitchWeaponVFXAsset.Succeeded()) SwitchWeaponVFX = SwitchWeaponVFXAsset.Object;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> BulletDefaultImpactAsset{ TEXT("/Game/ZombieSurvivals/Effects/ParticleSystems/Weapons/AssaultRifle/Impacts/VFX_Impact_Default.VFX_Impact_Default") };
	if (BulletDefaultImpactAsset.Succeeded()) BulletDefaultImpact = BulletDefaultImpactAsset.Object;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> BulletBodyImpactAsset{ TEXT("/Game/ZombieSurvivals/Effects/ParticleSystems/Gameplay/Player/P_body_bullet_impact.P_body_bullet_impact") };
	if (BulletBodyImpactAsset.Succeeded()) BulletBodyImpact = BulletBodyImpactAsset.Object;

}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	ZSGameState = Cast<AZombieSurvivalGameState>(GetWorld()->GetGameState()); 
}

void AWeaponBase::PlayerAction()
{
	ZSPlayer->OnPlayerInteractWithWeapon(WeaponData, WeaponState);
}


// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBase::OnEquippedWeapon(AZS_Player* NewOwner, UWeaponData* NewWeaponData)
{
	ZSPlayer = NewOwner;
	ZSPlayerState = Cast<AZombieSurvivalPlayerState>(ZSPlayer->GetPlayerState());
	WeaponState = EWeaponState::Holding;
	WeaponData = NewWeaponData;
	if (IsValid(WeaponData->WeaponMesh)) 
	{
		WeaponMeshComponent->SetStaticMesh(WeaponData->WeaponMesh);		
		if (WeaponMeshComponent->DoesSocketExist(MuzzleSocket)) 
		{
			MuzzleVFX->SetRelativeTransform(WeaponMeshComponent->GetSocketTransform(MuzzleSocket, RTS_Component));
		}
	}

	if (IsValid(WeaponData->BulletMuzzle))
	{
		MuzzleVFX->SetTemplate(WeaponData->BulletMuzzle);
	}

	const UObject* WorldContextObject = GetWorld();
	UGameplayStatics::PlaySoundAtLocation(WorldContextObject, PickupWeaponVFX, GetActorLocation());

	// Default Ammo 
	CurrentAmmo = WeaponData->AmmoPerMag;

	// Set Character MovementSpeed
	ZSPlayer->UpdateMovementSpeed(WeaponData->MovementSpeed);
}

void AWeaponBase::OnSwitchWeapon()
{
	WeaponState = EWeaponState::Holding;
	ActiveWeapon(true);
	const UObject* WorldContextObject = GetWorld();
	UGameplayStatics::PlaySoundAtLocation(WorldContextObject, SwitchWeaponVFX, GetActorLocation());
	// Set Character MovementSpeed
	ZSPlayer->UpdateMovementSpeed(WeaponData->MovementSpeed);
	WeaponCheckEmpty();
}

void AWeaponBase::OnStoredWeapon()
{
	WeaponEndFire();
	if (WeaponState == EWeaponState::Reloading)
	{
		GetWorldTimerManager().ClearTimer(ReloadTimerHandle);
		ZSPlayer->GetMesh()->GetAnimInstance()->Montage_Stop(0.1f, WeaponData->ReloadAnimation);
	}
	ActiveWeapon(false);
	WeaponState = EWeaponState::Storing;	
}

void AWeaponBase::ActiveWeapon(bool bActive)
{
	SetActorEnableCollision(bActive);
	SetActorHiddenInGame(!bActive);
	PrimaryActorTick.bCanEverTick = bActive;
}



void AWeaponBase::WeaponFire()
{
	isFiringCached = true;
	if (WeaponState == EWeaponState::Holding)
	{
		WeaponState = EWeaponState::Firing;
		GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AWeaponBase::FiringWeapon, WeaponData->FireRate, true, 0);
	}
}

void AWeaponBase::WeaponCharge()
{
	ZSPlayer->ShowCursorVFX();
}

void AWeaponBase::WeaponFireCharge()
{
	if (WeaponState == EWeaponState::Holding)
	{
		WeaponState = EWeaponState::Firing;
		FiringWeapon();
		GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AWeaponBase::WeaponEndFire, 0.01f, false, WeaponData->FireRate);
	}
}


void AWeaponBase::WeaponEndFire()
{
	isFiringCached = false;
	if (WeaponState == EWeaponState::Firing) 
	{
		WeaponState = EWeaponState::Holding;
	}
	if (WeaponData->WeaponType != EWeaponType::GrenadeLauncher)
		GetWorldTimerManager().ClearTimer(FireTimerHandle);
	else 
		WeaponCheckEmpty();
}


void AWeaponBase::WeaponCheckEmpty()
{
	if (CurrentAmmo <= 0 && (ZSPlayerState->GetTotalAmmo(WeaponData->WeaponType) > 0 || WeaponData->bIsInfiniteAmmo))
		StartReloadWeapon();
}

void AWeaponBase::ManualWeaponReload()
{
	if (WeaponState == EWeaponState::Reloading) return;
	if (CurrentAmmo < WeaponData->AmmoPerMag && (ZSPlayerState->GetTotalAmmo(WeaponData->WeaponType) > 0 || WeaponData->bIsInfiniteAmmo))
		StartReloadWeapon();
}

void AWeaponBase::FiringWeapon()
{
	if (IsValid(WeaponMeshComponent) && WeaponMeshComponent->DoesSocketExist(MuzzleSocket)) {

		//Minus Ammo
		if (CurrentAmmo > 0)
		{
			CurrentAmmo--;
			if (WeaponData->WeaponType == EWeaponType::GrenadeLauncher)
				WeaponFireOnSpawnProjectiles();
			else
				WeaponFireOnLineTrace();
		}
		else 
			WeaponFireEmpty();
	}
	else 
		WeaponEndFire();
}



void AWeaponBase::WeaponFireEmpty()
{
	FVector TraceStart = WeaponMeshComponent->GetSocketLocation(MuzzleSocket);

	if (CurrentAmmo <= 0) {
		const UObject* WorldContextObject = GetWorld();
		UGameplayStatics::PlaySoundAtLocation(WorldContextObject, WeaponData->FireSoundEmpty, TraceStart);
		return;
	}
}


void AWeaponBase::WeaponFireOnLineTrace()
{
	FHitResult Hit;

	FVector TraceStart = WeaponMeshComponent->GetSocketLocation(MuzzleSocket);

	FVector TraceEnd = TraceStart + (GetActorForwardVector() * 1000);
	//FVector TraceEnd = ZSPlayer->GetMouseLocation(); 
	// If we hit a surface, cache the location
	/*if (TraceEnd == FVector::ZeroVector)
	{
		TraceEnd = TraceStart + (GetActorForwardVector() * 1000);
	}
	else
	{
		TraceEnd.Z = TraceStart.Z;
		FVector NewDirection = TraceEnd - TraceStart;
		TraceEnd = ((NewDirection.Dot(GetActorForwardVector()) > 1) ? 1 : -1) * (TraceEnd - TraceStart) * 1000;
	}*/

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, QueryParams);
	//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, Hit.bBlockingHit ? FColor::Green : FColor::Red, false, 5.0f, 0, 10.0f);

	if (IsValid(WeaponData->BulletTrailEffect))
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(TraceStart);
		SpawnTransform.SetRotation(FQuat::Identity);
		SpawnTransform.SetScale3D(FVector::OneVector);
		const UObject* WorldContextObject = GetWorld();
		UGameplayStatics::PlaySoundAtLocation(WorldContextObject, WeaponData->FireSound, TraceStart);
		UParticleSystemComponent* BulletTrailVFX = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WeaponData->BulletTrailEffect, SpawnTransform);
		BulletTrailVFX->SetBeamSourcePoint(0, TraceStart, 0);
		BulletTrailVFX->SetVectorParameter("ShockBeamEnd", Hit.bBlockingHit ? Hit.ImpactPoint : TraceEnd);

		if (Hit.bBlockingHit) 
		{
			SpawnTransform.SetLocation(Hit.ImpactPoint);
			SpawnTransform.SetRotation(UKismetMathLibrary::MakeRotFromX(Hit.ImpactNormal).Quaternion());
			const bool isHitEnemy = Hit.GetActor()->ActorHasTag(TEXT("Enemy"));
			UParticleSystem* BulletImpact = (isHitEnemy) ?
				BulletBodyImpact : BulletDefaultImpact;

			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletImpact, SpawnTransform, true, EPSCPoolMethod::AutoRelease);

			//Deal Damage
			if (isHitEnemy) 
			{
				AController* PlayerController = Owner->GetInstigatorController();
				UGameplayStatics::ApplyPointDamage(Hit.GetActor(), WeaponData->BaseDamage,Hit.ImpactPoint, Hit, PlayerController, Owner, UDamageType::StaticClass());
			}
		}
	}
	if (IsValid(WeaponData->BulletMuzzle))
	{
		MuzzleVFX->ActivateSystem();
	}
	// Active action on player
	PlayerAction();
	// Check Reload
	WeaponCheckEmpty();
}


void AWeaponBase::WeaponFireOnSpawnProjectiles()
{
	if (WeaponData->BulletBase == nullptr)
		return;

	UClass* SpawnClass = WeaponData->BulletBase;
	FVector Location = WeaponMeshComponent->GetSocketLocation(MuzzleSocket);
	FRotator Rotation = WeaponMeshComponent->GetSocketRotation(MuzzleSocket);
	ABulletBase* Bullet = GetWorld()->GetSubsystem<UPoolSubsystem>()->SpawnFromPool<ABulletBase>(SpawnClass, Location, Rotation);
	Bullet->SetOwner(Owner);
	Bullet->StartMoving(ZSPlayer->GetMouseLocation(), WeaponData->BaseDamage);

	const UObject* WorldContextObject = GetWorld();
	UGameplayStatics::PlaySoundAtLocation(WorldContextObject, WeaponData->FireSound, Location);

	if (IsValid(WeaponData->BulletMuzzle))
	{
		MuzzleVFX->ActivateSystem();
	}

}


void AWeaponBase::StartReloadWeapon()
{
	WeaponState = EWeaponState::Reloading;
	GetWorldTimerManager().ClearTimer(FireTimerHandle);
	GetWorldTimerManager().SetTimer(ReloadTimerHandle, this, &AWeaponBase::ReloadWeapon, 0.1f, false, WeaponData->ReloadSpeed);
	PlayerAction();
}


void AWeaponBase::ReloadWeapon()
{
	GetWorldTimerManager().ClearTimer(ReloadTimerHandle);
	if(WeaponData->bIsInfiniteAmmo)
		CurrentAmmo = WeaponData->AmmoPerMag;
	else 
	{
		int needAmmo = FMath::Min(ZSPlayerState->GetTotalAmmo(WeaponData->WeaponType), WeaponData->AmmoPerMag - CurrentAmmo);
		CurrentAmmo += needAmmo;
		ZSPlayerState->CalculateAmmo(WeaponData->WeaponType, needAmmo);
	}
	WeaponState = EWeaponState::Holding;
	if (isFiringCached && WeaponData->WeaponType != EWeaponType::GrenadeLauncher)
	{
		WeaponFire();
	}
}



