// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponItem.h"
#include "WeaponBase.h"
#include "ZombieSurvival/Character/ZS_Player.h"

AWeaponItem::AWeaponItem()
{
	MagMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MagMeshComponent"));
	MagMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
	MagMeshComponent->SetupAttachment(MeshComponent);
}

void AWeaponItem::BeginPlay()
{
	Super::BeginPlay();
	OnItemSpawn();
}



void AWeaponItem::Tick(float DeltaTime)
{
}

void AWeaponItem::OnItemSpawn()
{
	if (IsValid(WeaponData->WeaponMesh))
	{
		MeshComponent->SetStaticMesh(WeaponData->WeaponMesh);
		TriggerSphere->SetWorldLocation(MeshComponent->Bounds.GetSphere().Center);
	}
	if (IsValid(WeaponData->WeaponMagMesh))
	{
		if(MeshComponent->DoesSocketExist(WeaponData->MagSocketName))
			MagMeshComponent->SetRelativeTransform(MeshComponent->GetSocketTransform(WeaponData->MagSocketName, RTS_ParentBoneSpace));
		MagMeshComponent->SetStaticMesh(WeaponData->WeaponMagMesh);
	}
}

void AWeaponItem::OnPlayerOverlap(AActor* PlayerActor)
{
	Super::OnPlayerOverlap(PlayerActor);
	if (AZS_Player* Player = CastChecked<AZS_Player>(PlayerActor)) 
	{
		AWeaponBase* NewWeapon = PoolSubsystem->SpawnFromPool<AWeaponBase>(AWeaponBase::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		if (IsValid(NewWeapon)) {
			Player->PickupWeapon(NewWeapon);
			NewWeapon->OnEquippedWeapon(Player, WeaponData);
		}
	};
	
}
