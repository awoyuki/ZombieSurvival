#include "ZS_Player.h"

// Sets default values
AZS_Player::AZS_Player()
{
	// Set this character to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AZS_Player::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AZS_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

