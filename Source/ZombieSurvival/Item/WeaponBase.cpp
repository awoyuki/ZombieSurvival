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

	BulletTrailVFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BulletTrailVFX"));
	BulletTrailVFX->SetupAttachment(WeaponMeshComponent);

}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
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
			BulletTrailVFX->SetRelativeTransform(WeaponMeshComponent->GetSocketTransform(MuzzleSocket, RTS_Component));
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
}

void AWeaponBase::WeaponFire()
{
	GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AWeaponBase::FiringWeapon, WeaponData.FireRate, true, 0);

}
void AWeaponBase::WeaponEndFire()
{
	GetWorldTimerManager().ClearTimer(FireTimerHandle);

}

void AWeaponBase::OnReturnToPool_Implementation()
{
	WeaponEndFire();
}

void AWeaponBase::FiringWeapon()
{
	FHitResult Hit;

	if (IsValid(WeaponMeshComponent) && WeaponMeshComponent->DoesSocketExist(MuzzleSocket)) {

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
			UGameplayStatics::PlaySound2D(GetWorld(), WeaponData.FireSound);
			BulletTrailVFX = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WeaponData.BulletTrailEffect, SpawnTransform);
			BulletTrailVFX->SetBeamSourcePoint(0, TraceStart, 0);
			BulletTrailVFX->SetVectorParameter("ShockBeamEnd", Hit.bBlockingHit ? Hit.ImpactPoint : TraceEnd);

			if (Hit.bBlockingHit) {
				SpawnTransform.SetLocation(Hit.ImpactPoint);
				SpawnTransform.SetRotation(UKismetMathLibrary::MakeRotFromX(Hit.ImpactNormal).Quaternion());
				BulletTrailVFX = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WeaponData.BulletImpact, SpawnTransform);
			}
		}
		if (IsValid(WeaponData.BulletMuzzle)) 
		{
			MuzzleVFX->ActivateSystem();
		}
		
		if (Owner->GetClass()->ImplementsInterface(UICharacter::StaticClass())) 
		{
			IICharacter* Player = Cast<IICharacter>(Owner);
			Player->OnPlayerFiringWeapon(WeaponData);
		}
		//Deal Damage
	
	}
}



