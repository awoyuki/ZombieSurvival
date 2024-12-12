// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZombieZone.h"
#include "GameFramework/Actor.h"
#include "LevelController.generated.h"


UENUM(BlueprintType)
enum class EItemType : uint8
{
	None UMETA(DisplayName = "None"),
	Health UMETA(DisplayName = "Health"),
	RilfeAmmo UMETA(DisplayName = "RilfeAmmo"),
	GrenadeAmmo UMETA(DisplayName = "GrenadeAmmo")
};

USTRUCT(BlueprintType)
struct FItemDropStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABaseItem> ItemClass;

	UPROPERTY(EditAnywhere)
	float DropRate;
};

UCLASS()
class ZOMBIESURVIVAL_API ALevelController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EItemType, FItemDropStruct> ItemContainer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AZombieZone*> ZombieSpawn;

	EItemType RollSingleItem();
	void SpawnRandomItem(FVector Location);

	UFUNCTION(BlueprintCallable)
	void OnPlayerGetOverZone();
};
