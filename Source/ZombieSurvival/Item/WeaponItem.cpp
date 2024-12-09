// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponItem.h"
#include "WeaponBase.h"
#include "ZombieSurvival/Character/ZS_Player.h"

AWeaponItem::AWeaponItem()
{
}

void AWeaponItem::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(RotateTimeHandle, this, &AWeaponItem::RotateMesh, 0.01f, true, 0);
}

void AWeaponItem::RotateMesh()
{
	AddActorWorldRotation(FRotator(0, 1, 0).Quaternion());
}



void AWeaponItem::Tick(float DeltaTime)
{	
}

void AWeaponItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (IsValid(WeaponData->WeaponMesh))
	{
		MeshComponent->SetStaticMesh(WeaponData->WeaponMesh);
		TriggerSphere->SetWorldLocation(MeshComponent->Bounds.GetSphere().Center);
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
