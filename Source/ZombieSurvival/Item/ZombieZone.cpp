// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieZone.h"
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

void AZombieZone::SpawnZombie(ALevelController* NewLevelOwner)
{
	LevelOwner = NewLevelOwner;
	if (TriggerBox != nullptr) {

		int NumberOfZombies = FMath::FRandRange(MaxNumberOfZombies/1.5f, MaxNumberOfZombies);

		UPoolSubsystem* PoolSubsystem = GetWorld()->GetSubsystem<UPoolSubsystem>();
		for (int i = 0; i < NumberOfZombies; i++)
		{
			FVector Location = FVector::Zero();
			TriggerBox->SetWorldRotation(FRotator::ZeroRotator);
			FVector RandomPoint = FMath::RandPointInBox(TriggerBox->Bounds.GetBox());
			FMatrix LocalToWorldInverse = GetActorTransform().ToMatrixWithScale();
			Location = UKismetMathLibrary::GreaterGreater_VectorRotator(LocalToWorldInverse.GetOrigin() - RandomPoint, LocalToWorldInverse.Rotator()) + GetActorLocation();
			Location.Z = GetActorLocation().Z;
			FRotator Rotation(0, FMath::FRandRange(-180.f, 180.f), 0);

			if (ZombieClasses.Num() == 0)
			{
				return;
			}

			int32 n = ZombieClasses.Num();

			int32 TotalWeight = n * (n + 1) / 2;
			int32 RandomValue = FMath::RandRange(1, TotalWeight);

			for (int32 j = 0; j < n; ++j)
			{
				int32 Weight = n - j;
				RandomValue -= Weight;
				if (RandomValue <= 0)
				{
					AZS_ZombieBase* NewZombie = PoolSubsystem->SpawnFromPool<AZS_ZombieBase>(ZombieClasses[j], Location, Rotation);
					NewZombie->SetOwner(this);
					NewZombie->OnSpawnEnemy(nullptr);
					ZombieList.Add(NewZombie);
					break;
				}
			}

		}
	}
}

void AZombieZone::KillZombie()
{
	for (auto Enemy: ZombieList)
	{
		Enemy->SetEnemyState(EEnemyState::Death);
	}
}


