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

void AWeaponItem::OnPlayerOverlap_Implementation(AActor* PlayerActor)
{
	Super::OnPlayerOverlap_Implementation(PlayerActor);
	if (PlayerActor->GetClass()->ImplementsInterface(UIZSCharacter::StaticClass()))
	{
		IIZSCharacter::Execute_PickUpWeapon(PlayerActor, WeaponData);
	}	
}
