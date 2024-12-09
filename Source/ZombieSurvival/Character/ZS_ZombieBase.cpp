
#include "Engine/DamageEvents.h"
#include "ZS_ZombieBase.h"

// Sets default values
AZS_ZombieBase::AZS_ZombieBase()
{
	// Set this empty to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0).Quaternion());
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

void AZS_ZombieBase::OnInitEnemy(UEnemyData* NewData)
{
	EnemyData = NewData;
	CurrentHealth = EnemyData->EnemyHealth;
}

float AZS_ZombieBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{		
	// Check If is Point Damage
	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		FPointDamageEvent* PointDamageEvent = (FPointDamageEvent*)&DamageEvent;
		if (PointDamageEvent)
		{
			FVector HitNormal = PointDamageEvent->HitInfo.ImpactNormal;
			CurrentHealth = FMath::Max(CurrentHealth - DamageAmount, 0);
			if (CurrentHealth <= 0)
			{
				OnEnemyDeath();
			}
			//Knockback
			LaunchCharacter(- HitNormal * DamageAmount * 10, true, true);
		}
	}
	return 0.0f;
}

void AZS_ZombieBase::OnEnemyDeath()
{
	GetMesh()->SetSimulatePhysics(true);
	GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &AZS_ZombieBase::OnEnemyRemoveFromWorld, 0.1f, false, 3.0f);
}

void AZS_ZombieBase::OnEnemyRemoveFromWorld()
{
	GetWorld()->GetSubsystem<UPoolSubsystem>()->ReturnToPool(this);
}

