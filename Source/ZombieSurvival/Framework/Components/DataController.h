// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ZombieSurvival/Data/CommonData.h"
#include "ZombieSurvival/Data/WeaponData.h"
#include "DataController.generated.h"



UCLASS( ClassGroup=(DataController), meta=(BlueprintSpawnableComponent) )
class ZOMBIESURVIVAL_API UDataController : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDataController();

	// Asset Data will be stored here
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data Assets")
	class UCommonData* CommonData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data Assets")
	class UWeaponData* WeaponData;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
