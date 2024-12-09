#include "ZS_ZombieBase.h"

// Sets default values
AZS_ZombieBase::AZS_ZombieBase()
{
	// Set this empty to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add(TEXT("Enemy"));
}

// Called when the game starts or when spawned
void AZS_ZombieBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AZS_ZombieBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

