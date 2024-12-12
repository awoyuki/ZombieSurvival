#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Character.h"
#include "ZombieSurvival/PoolingSystem/PoolSubsystem.h"
#include "ZombieSurvival/Interface/IZSEnemy.h"
#include "ZombieSurvival/Data/EnemyData.h"
#include "ZS_ZombieBase.generated.h"


UCLASS(Blueprintable)
class ZOMBIESURVIVAL_API AZS_ZombieBase : public ACharacter, public IPoolable, public IIZSEnemy
{
	GENERATED_BODY()

public:
	// Sets default values for this empty's properties
	AZS_ZombieBase();

	//Widget Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy Property")
	class UWidgetComponent* HealthBar;

	FTimerHandle DeathTimerHandle;

	//Stored Data
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Property")
	UEnemyData* EnemyData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy Property")
	int CurrentHealth = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy Property")
	EEnemyState CurrentState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Property")
	TArray<class UEnemyAbilityBase*> Abilities;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Property")
	class UEnemyAbilityBase* CurrentAbility;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy Property")
	float CurrentDamage = 10;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy Property")
	float CurrentAttackRange = 100;

	virtual float GetCurrentAttackRange_Implementation() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform &Transform)override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy AI")
	UBehaviorTree* Tree;

	// Cached Framework
	class AZS_AIController* AIZSController;
	//Cached Value
	FTransform InitPosition;

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnInitEnemy(bool bIsContruction);

	virtual void OnSpawnEnemy(UEnemyData* NewData);

	virtual void SetEnemyState(EEnemyState newState);

	virtual bool DoesCurrentMontageFinish_Implementation() override;

	virtual EEnemyState GetEnemyState_Implementation() override;

	virtual void SetEnemyStateInterface_Implementation(EEnemyState newState) override;

	virtual float GetCurrentAttackAnimationDuration_Implementation() override;

	float PlayAnimMontage(class UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void OnEnemyRemoveFromWorld();	

	UBehaviorTree* GetBehaviorTree() const;

	virtual void SelectAbility();

	virtual void EnemyPatrol();

	virtual void EnemyChase();

	virtual void EnemyAttack();

	virtual void EnemyDeath();

	UFUNCTION(BlueprintCallable, Category = "Enemy Actions")
	void EnemyAttacking();


	UBlackboardComponent* GetBlackBoardFromAIController();

	UFUNCTION(BlueprintCallable, Category = "Enemy Ability", meta = (DeterminesOutputType = "AbilityClass", DynamicOutputParam = "Ability"))
	void CreateAbility(TSubclassOf<UEnemyAbilityBase> AbilityClass, int index, UEnemyAbilityBase*& Ability);

	template<typename T>
	T* CreateAbility(TSubclassOf<UEnemyAbilityBase> AbilityClass, int index);


};

template<typename T>
inline T* AZS_ZombieBase::CreateAbility(TSubclassOf<UEnemyAbilityBase> AbilityClass, int index)
{
	T* Ability = NewObject<T>(this, AbilityClass, FName(*FString::Printf(TEXT("Ability%d"), (index + 1))));
	if (Ability)
	{
		Ability->CreationMethod = EComponentCreationMethod::Instance;
		Ability->RegisterComponent();
		AddInstanceComponent(Ability);
	}	
	return Ability;
}