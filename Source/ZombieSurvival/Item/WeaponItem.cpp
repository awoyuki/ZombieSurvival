// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponItem.h"

AWeaponItem::AWeaponItem()
{
}

void AWeaponItem::BeginPlay()
{
	Super::BeginPlay();
}



void AWeaponItem::Tick(float DeltaTime)
{
}

void AWeaponItem::OnItemSpawn()
{
	MeshComponent->SetStaticMesh(WeaponData.WeaponMesh);
}
