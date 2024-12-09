#include "WeaponBase.h"
#include "Particles/ParticleSystemComponent.h"
#include "ZombieSurvival/Interface/ICharacter.h"
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
	MagMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MagMeshComponent"));
	MagMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
	MagMeshComponent->SetupAttachment(WeaponMeshComponent);

	MuzzleVFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MuzzleVFX"));
	MuzzleVFX->SetupAttachment(WeaponMeshComponent);
	MuzzleVFX->SetAutoActivate(false);


}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	ZSGameState = Cast<AZombieSurvivalGameState>(GetWorld()->GetGameState());
}

void AWeaponBase::PlayerAction()
{
	if (Owner->GetClass()->ImplementsInterface(UICharacter::StaticClass()))
	{
		IICharacter* Player = Cast<IICharacter>(Owner);
		Player->OnPlayerInteractWithWeapon(WeaponData, WeaponState);
	}
}


// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBase::OnEquippedWeapon(AZS_Player* NewOwner, FWeaponDataStruct NewWeaponData)
{
	WeaponData = NewWeaponData;
	if (IsValid(WeaponData.WeaponMesh)) 
	{
		WeaponMeshComponent->SetStaticMesh(WeaponData.WeaponMesh);
		if (WeaponMeshComponent->DoesSocketExist(WeaponData.MagSocketName))
			MagMeshComponent->SetRelativeTransform(WeaponMeshComponent->GetSocketTransform(WeaponData.MagSocketName, RTS_Component));
		if (WeaponMeshComponent->DoesSocketExist(MuzzleSocket)) 
		{
			MuzzleVFX->SetRelativeTransform(WeaponMeshComponent->GetSocketTransform(MuzzleSocket, RTS_Component));
		}
	}
	if (IsValid(WeaponData.WeaponMagMesh)) 
	{
		MagMeshComponent->SetStaticMesh(WeaponData.WeaponMagMesh);
	}
	if (IsValid(WeaponData.BulletMuzzle))
	{
		MuzzleVFX->SetTemplate(WeaponData.BulletMuzzle);
	}

	WeaponState = EWeaponState::Holding;

	// Default Ammo 
	CurrentAmmo = WeaponData.AmmoPerMag;
	if(!WeaponData.bIsInfiniteAmmo)
		TotalAmmo = WeaponData.AmmoPerMag * 3;
}

void AWeaponBase::WeaponFire()
{
	isFiringCached = true;
	if (WeaponState == EWeaponState::Holding)
	{
		WeaponState = EWeaponState::Firing;
		GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AWeaponBase::FiringWeapon, WeaponData.FireRate, true, 0);
	}

}
void AWeaponBase::WeaponEndFire()
{
	isFiringCached = false;
	if (WeaponState == EWeaponState::Firing) 
	{
		WeaponState = EWeaponState::Holding;
	}
	GetWorldTimerManager().ClearTimer(FireTimerHandle);

}

void AWeaponBase::OnReturnToPool_Implementation()
{
	WeaponEndFire();
	WeaponState = EWeaponState::Storing;
}

void AWeaponBase::FiringWeapon()
{
	if (IsValid(WeaponMeshComponent) && WeaponMeshComponent->DoesSocketExist(MuzzleSocket)) {

		//Minus Ammo
		if (CurrentAmmo <= 0)
		{
			if (TotalAmmo > 0)
				StartReloadWeapon();
			else
				WeaponFireEmpty();
		}
		else 
		{
			CurrentAmmo--;
			if (WeaponData.WeaponType == EWeaponType::GrenadeLauncher)
				WeaponFireOnSpawnProjectiles();
			else
				WeaponFireOnLineTrace();
		}
	}
	else 
	{
		WeaponEndFire();
	}
}

void AWeaponBase::WeaponFireEmpty()
{
	FVector TraceStart = WeaponMeshComponent->GetSocketLocation(MuzzleSocket);

	if (CurrentAmmo <= 0) {
		const UObject* WorldContextObject = GetWorld();
		UGameplayStatics::PlaySoundAtLocation(WorldContextObject, WeaponData.FireSoundEmpty, TraceStart);
		return;
	}
}


void AWeaponBase::WeaponFireOnLineTrace()
{
	FHitResult Hit;

	FVector TraceStart = WeaponMeshComponent->GetSocketLocation(MuzzleSocket);

	FHitResult MouseHit;
	bool bHitSuccessful = false;
	bHitSuccessful = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHitResultUnderCursor(ECC_Visibility, true, MouseHit);
	FVector TraceEnd = TraceStart + (GetActorForwardVector() * 1000);
	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		TraceEnd = MouseHit.ImpactPoint;
		TraceEnd.Z = TraceStart.Z;
		FVector NewDirection = TraceEnd - TraceStart;
		TraceEnd = ((NewDirection.Dot(GetActorForwardVector()) > 1) ? 1 : -1) * (TraceEnd - TraceStart) * 1000;
	}

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, QueryParams);
	//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, Hit.bBlockingHit ? FColor::Green : FColor::Red, false, 5.0f, 0, 10.0f);

	if (IsValid(WeaponData.BulletTrailEffect))
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(TraceStart);
		SpawnTransform.SetRotation(FQuat::Identity);
		SpawnTransform.SetScale3D(FVector::OneVector);
		const UObject* WorldContextObject = GetWorld();
		UGameplayStatics::PlaySoundAtLocation(WorldContextObject, WeaponData.FireSound, TraceStart);
		UParticleSystemComponent* BulletTrailVFX = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WeaponData.BulletTrailEffect, SpawnTransform);
		BulletTrailVFX->SetBeamSourcePoint(0, TraceStart, 0);
		BulletTrailVFX->SetVectorParameter("ShockBeamEnd", Hit.bBlockingHit ? Hit.ImpactPoint : TraceEnd);

		if (Hit.bBlockingHit) {
			SpawnTransform.SetLocation(Hit.ImpactPoint);
			SpawnTransform.SetRotation(UKismetMathLibrary::MakeRotFromX(Hit.ImpactNormal).Quaternion());

			UParticleSystem* BulletImpact = (Hit.GetActor()->ActorHasTag(TEXT("Enemy"))) ?
				ZSGameState->DataController->CommonData->BulletBodyImpact : ZSGameState->DataController->CommonData->BulletDefaultImpact;

			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletImpact, SpawnTransform, true, EPSCPoolMethod::AutoRelease);

		}
	}
	if (IsValid(WeaponData.BulletMuzzle))
	{
		MuzzleVFX->ActivateSystem();
	}
	// Active action on player
	PlayerAction();
	//Deal Damage
}


void AWeaponBase::WeaponFireOnSpawnProjectiles()
{

}


void AWeaponBase::StartReloadWeapon()
{
	WeaponState = EWeaponState::Reloading;
	GetWorldTimerManager().ClearTimer(FireTimerHandle);
	GetWorldTimerManager().SetTimer(ReloadTimerHandle, this, &AWeaponBase::ReloadWeapon, 0.1f, false, WeaponData.ReloadSpeed);
	PlayerAction();
}


void AWeaponBase::ReloadWeapon()
{
	GetWorldTimerManager().ClearTimer(ReloadTimerHandle);
	if(WeaponData.bIsInfiniteAmmo)
		CurrentAmmo = WeaponData.AmmoPerMag;
	else {
		CurrentAmmo = fmin(TotalAmmo, WeaponData.AmmoPerMag);
		TotalAmmo = fmax(TotalAmmo - CurrentAmmo, 0);
	}
	WeaponState = EWeaponState::Holding;
	if (isFiringCached) {
		WeaponFire();
	}
}



