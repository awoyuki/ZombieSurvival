// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieZone.h"
#include "ZombieSurvival/Character/ZS_ZombieBase.h"
#include "ZombieSurvival/PoolingSystem/PoolSubsystem.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AZombieZone::AZombieZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	SetRootComponent(TriggerBox);

	//Just for bounds
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AZombieZone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AZombieZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AZombieZone::SpawnZombie(int NumberOfZombies)
{
	if (TriggerBox != nullptr) {
		UPoolSubsystem* PoolSubsystem = GetWorld()->GetSubsystem<UPoolSubsystem>();
		for (int i = 0; i < NumberOfZombies; i++)
		{
			FVector Location = FVector::Zero();
			TriggerBox->SetWorldRotation(FRotator::ZeroRotator);
			FVector RandomPoint = FMath::RandPointInBox(TriggerBox->Bounds.GetBox());
			FMatrix LocalToWorldInverse = GetActorTransform().ToMatrixWithScale();
			Location = UKismetMathLibrary::GreaterGreater_VectorRotator(LocalToWorldInverse.GetOrigin() - RandomPoint, LocalToWorldInverse.Rotator()) + GetActorLocation();

			FRotator Rotation(0, FMath::FRandRange(-180.f, 180.f), 0);

			AZS_ZombieBase* NewZombie = PoolSubsystem->SpawnFromPool<AZS_ZombieBase>(AZS_ZombieBase::StaticClass(), Location, Rotation);
		}
	}
}


