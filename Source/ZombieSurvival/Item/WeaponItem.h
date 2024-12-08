// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZombieSurvival/Data/WeaponData.h"
#include "BaseItem.h"
#include "WeaponItem.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESURVIVAL_API AWeaponItem : public ABaseItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeaponItem();

	// Stored Data
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FWeaponDataStruct WeaponData;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnItemSpawn() override;
};