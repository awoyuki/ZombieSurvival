
#include "ZS_ZombieBase.h"
#include "Ability/EnemyAbilityBase.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ZombieSurvival/AI/ZS_AIController.h"
#include "ZombieSurvival/UserWidget/HealthBarUI.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include <ZombieSurvival/Item/LevelController.h>

// Sets default values
AZS_ZombieBase::AZS_ZombieBase()
{
	// Set this empty to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AZS_AIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0).Quaternion());
	Tags.Add(TEXT("Enemy"));

	//Bind widget
	HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBar->SetupAttachment(RootComponent);
	HealthBar->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBar->SetRelativeLocation(FVector(0, 0, 90));
	HealthBar->SetDrawSize(FVector2D(80, 10));
	static ConstructorHelpers::FClassFinder<UUserWidget> HealthBarClass{ TEXT("/Game/ZombieSurvivals/Blueprints/Widget/UIHealthBar") }; 
	if (HealthBarClass.Succeeded()) 
	{
		HealthBar->SetWidgetClass((HealthBarClass.Class));
	}
}

void AZS_ZombieBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	OnInitEnemy(false);
}

// Called when the game starts or when spawned
void AZS_ZombieBase::BeginPlay()
{
	Super::BeginPlay();
	AIZSController = Cast<AZS_AIController>(GetController());
	OnSpawnEnemy(EnemyData);
}

// Called every frame
void AZS_ZombieBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (EnemyData == nullptr) return;
	if (HealthBar->GetUserWidgetObject() == nullptr) return;
	if (auto const widget = CastChecked<UHealthBarUI>(HealthBar->GetUserWidgetObject()))
	{
		widget->SetBarValuePercent(CurrentHealth / EnemyData->Health);
	}
}

void AZS_ZombieBase::OnInitEnemy(bool bIsContruction)
{
	if (EnemyData == nullptr)
		return;

	CurrentHealth = EnemyData->Health;
	GetMesh()->ResetAllBodiesSimulatePhysics();
	GetMesh()->SetSkeletalMesh(EnemyData->Mesh);	

	if (!bIsContruction)
	{
		Abilities.Empty();
		auto InitAbilities = GetComponentsByClass(UEnemyAbilityBase::StaticClass());
		for (int i = 0; i < InitAbilities.Num(); i++)
		{
			if (auto Ability = Cast<UEnemyAbilityBase>(InitAbilities[i]))
				Abilities.Add(Ability);
		}
	}
}

void AZS_ZombieBase::OnSpawnEnemy(UEnemyData* NewData)
{
	EnemyData = NewData;
	OnInitEnemy(false);
	HealthBar->SetVisibility(true);
	CurrentState = EEnemyState::Idle;
	SelectAbility();
}

void AZS_ZombieBase::SetEnemyState(EEnemyState newState)
{
	if (CurrentState == newState)
		return;

	CurrentState = newState;
	switch (CurrentState)
	{
	case EEnemyState::Idle:SelectAbility();
		break;
	case EEnemyState::Patrol:EnemyPatrol();
		break;
	case EEnemyState::Chasing:EnemyChase();
		break;
	case EEnemyState::Attack:EnemyAttack();
		break;
	case EEnemyState::Death:EnemyDeath();
		break;
	}
}

void AZS_ZombieBase::SelectAbility()
{
	if (Abilities.Num() == 0)
	{
		SetEnemyState(EEnemyState::Idle);
		return;
	}

	int32 n = Abilities.Num();

	int32 TotalWeight = n * (n + 1) / 2;
	int32 RandomValue = FMath::RandRange(1, TotalWeight);

	for (int32 i = 0; i < n; ++i)
	{
		int32 Weight = n - i;
		RandomValue -= Weight;
		if (RandomValue <= 0)
		{
			CurrentAbility = Abilities[i];
			CurrentAbility->OnSelectAbility();
			return;
		}
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
	if(CurrentAbility)
		CurrentAbility->ActiveAbility();

}
void AZS_ZombieBase::EnemyDeath()
{
	AIZSController->GetBlackboardComponent()->SetValueAsEnum("EnemyState", (uint8)CurrentState);
	GetMesh()->SetSimulatePhysics(true);
	GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &AZS_ZombieBase::OnEnemyRemoveFromWorld, 0.1f, false, 3.0f);
	GetMesh()->GetAnimInstance()->Montage_Stop(0.1f);
	HealthBar->SetVisibility(false);
	//Call to spawn item 
	if (auto* ZombieZone = CastChecked<AZombieZone>(GetOwner()))
	{
		if (ZombieZone->LevelOwner)
			ZombieZone->LevelOwner->SpawnRandomItem(GetActorLocation());
	}
}

float AZS_ZombieBase::GetCurrentAttackAnimationDuration()
{
	UAnimInstance* AnimInstance = (GetMesh()) ? GetMesh()->GetAnimInstance() : nullptr;
	if (AnimInstance)
	{
		UAnimMontage* Montage = AnimInstance->GetCurrentActiveMontage();
		if (Montage) 
		{
			return Montage->GetPlayLength() * AnimInstance->Montage_GetPlayRate(Montage);
		}
	}
	return 0;
}

bool AZS_ZombieBase::DoesCurrentMontageFinish()
{
	UAnimInstance* AnimInstance = (GetMesh()) ? GetMesh()->GetAnimInstance() : nullptr;
	if (AnimInstance)
	{
		UAnimMontage* Montage = AnimInstance->GetCurrentActiveMontage();
		if (Montage)
		{
			return AnimInstance->Montage_GetIsStopped(Montage);
		}
	}
	return true;
}

UBlackboardComponent* AZS_ZombieBase::GetBlackBoardFromAIController()
{
	if(AIZSController)
		return AIZSController->GetBlackboardComponent();
	return nullptr;
}

void AZS_ZombieBase::EnemyAttacking()
{
	CurrentAbility->EndAbility();
	CurrentAbility = nullptr;
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
		SetEnemyState(EEnemyState::Death);
	}
	return 0.0f;
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

void AZS_ZombieBase::CreateAbility(TSubclassOf<UEnemyAbilityBase> AbilityClass, int index, UEnemyAbilityBase*& Ability)
{
	Ability = CreateAbility<UEnemyAbilityBase>(AbilityClass, index);
}