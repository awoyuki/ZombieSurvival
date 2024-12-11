// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelController.h"
#include "Kismet/GameplayStatics.h"
#include "BaseItem.h"

// Sets default values
ALevelController::ALevelController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}


// Called when the game starts or when spawned
void ALevelController::BeginPlay()
{
	Super::BeginPlay();	
    for (auto ZombieZone : ZombieSpawn)
    {
        ZombieZone->SpawnZombie(this);
    }
}

// Called every frame
void ALevelController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


EItemType ALevelController::RollSingleItem()
{
    float TotalRate = 0.0f;

    
    for (const auto& Item : ItemContainer) {
        TotalRate += Item.Value.DropRate;
    }
    
    float Roll = FMath::FRandRange(0.0f, TotalRate);
    float CurrentRate = 0.0f;

    for (const auto& Item : ItemContainer) {
        CurrentRate += Item.Value.DropRate;
        if (Roll <= CurrentRate) {
            return Item.Key; 
        }
    }

    return EItemType::None;
}

void ALevelController::SpawnRandomItem(FVector Location)
{
    auto ItemType = RollSingleItem();
    if (ItemType == EItemType::None)
        return;

    auto ClassType = ItemContainer.Find(ItemType);
    if(ClassType == nullptr)
        return;

    auto Item = GetWorld()->GetSubsystem<UPoolSubsystem>()->SpawnFromPool<ABaseItem>(ClassType->ItemClass, Location, FRotator::ZeroRotator);
    Item->OnItemSpawn();
}