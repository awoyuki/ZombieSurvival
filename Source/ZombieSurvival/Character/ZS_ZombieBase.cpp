
#include "ZS_ZombieBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ZombieSurvival/AI/ZS_AIController.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AZS_ZombieBase::AZS_ZombieBase()
{
	// Set this empty to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AZS_AIController::StaticClass();

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0).Quaternion());
	Tags.Add(TEXT("Enemy"));
}

void AZS_ZombieBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	OnInitEnemy();
}

// Called when the game starts or when spawned
void AZS_ZombieBase::BeginPlay()
{
	Super::BeginPlay();
	AIZSController = Cast<AZS_AIController>(GetController());
}

// Called every frame
void AZS_ZombieBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AZS_ZombieBase::OnInitEnemy()
{
	if (EnemyData == nullptr)
		return;

	CurrentHealth = EnemyData->Health;
	GetMesh()->ResetAllBodiesSimulatePhysics();
	GetMesh()->SetSkeletalMesh(EnemyData->Mesh);	
}

void AZS_ZombieBase::OnSpawnEnemy(UEnemyData* NewData)
{
	EnemyData = NewData;
	OnInitEnemy();
	CurrentState = EEnemyState::Idle;
}

void AZS_ZombieBase::SetEnemyState(EEnemyState newState)
{
	if (CurrentState == newState)
		return;

	CurrentState = newState;
	switch (CurrentState)
	{
	case EEnemyState::Patrol:EnemyPatrol();
		break;
	case EEnemyState::Chasing:EnemyChase();
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("Chasing"));
		break;
	case EEnemyState::Attack:EnemyAttack();
		break;
	case EEnemyState::Death:
		break;
	}
}

void AZS_ZombieBase::EnemyPatrol()
{
	GetCharacterMovement()->MaxWalkSpeed = 125;
	GetMesh()->GetAnimInstance()->Montage_Stop(0.1f);
}
void AZS_ZombieBase::EnemyChase()
{
	GetCharacterMovement()->MaxWalkSpeed = EnemyData ? EnemyData->MovementSpeed : 300;
	GetMesh()->GetAnimInstance()->Montage_Stop(0.1f);
}
void AZS_ZombieBase::EnemyAttack()
{
	PlayAnimMontage(EnemyData->AttackAnimation, 1, NAME_None);
}
void AZS_ZombieBase::EnemyDeath()
{
}


void AZS_ZombieBase::EnemyAttacking()
{
	if (GetMesh()->DoesSocketExist("weapon_r_socket"))
	{
		FHitResult Hit;
		FCollisionShape ColShape = FCollisionShape::MakeSphere(EnemyData->AttackRange);
		FVector Location = GetMesh()->GetSocketLocation("weapon_r_socket");
		GetWorld()->SweepSingleByChannel(Hit, Location, Location, FQuat::Identity, ECC_Camera, ColShape);

		if (Hit.bBlockingHit)
		{
			if (Hit.GetActor()->ActorHasTag("Player"))
			{
				AController* EnemyController = Owner->GetInstigatorController();
				UGameplayStatics::ApplyPointDamage(Hit.GetActor(), EnemyData->BaseDamage, Hit.ImpactPoint, Hit, EnemyController, this, nullptr);
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("Hit Player!"));
			}
		}
	}
	SetEnemyState(EEnemyState::Idle);
}


float AZS_ZombieBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{		
	FVector HitNormal = FVector::ZeroVector;
	// Check If is Point Damage
	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		FPointDamageEvent* PointDamageEvent = (FPointDamageEvent*)&DamageEvent;
		if (PointDamageEvent)
		{
			HitNormal = - PointDamageEvent->HitInfo.ImpactNormal;
		}
	}
	if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
	{
		FRadialDamageEvent* RaialDamageEvent = (FRadialDamageEvent*)&DamageEvent;
		if (RaialDamageEvent)
		{
			HitNormal = (GetActorLocation() - RaialDamageEvent->Origin).GetSafeNormal();
		}
	}
	//Knockback
	LaunchCharacter(HitNormal * DamageAmount * 10, true, true);
	CurrentHealth = FMath::Max(CurrentHealth - DamageAmount, 0);
	//if Player -> chase to him
	if (DamageCauser->ActorHasTag("Player")) 
	{
		AIZSController->GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", true);
	}

	if (CurrentHealth <= 0)
	{
		OnEnemyDeath();
	}
	return 0.0f;
}

void AZS_ZombieBase::OnEnemyDeath()
{
	GetMesh()->SetSimulatePhysics(true);
	GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &AZS_ZombieBase::OnEnemyRemoveFromWorld, 0.1f, false, 3.0f);
	SetEnemyState(EEnemyState::Death);
}

void AZS_ZombieBase::OnEnemyRemoveFromWorld()
{
	GetWorld()->GetSubsystem<UPoolSubsystem>()->ReturnToPool(this);
}

UBehaviorTree* AZS_ZombieBase::GetBehaviorTree() const
{
	return Tree;
}


float AZS_ZombieBase::PlayAnimMontage(class UAnimMontage* AnimMontage, float InPlayRate = 1, FName StartSectionName = NAME_None)
{
	UAnimInstance* AnimInstance = (GetMesh()) ? GetMesh()->GetAnimInstance() : nullptr;
	if (AnimMontage && AnimInstance)
	{
		float const Duration = AnimInstance->Montage_Play(AnimMontage, InPlayRate);
		if (Duration > 0.f)
		{
			// Start at a given Section.
			if (StartSectionName != NAME_None)
			{
				AnimInstance->Montage_JumpToSection(StartSectionName, AnimMontage);
			}
			return Duration;
		}
	}
	return 0.f;
}