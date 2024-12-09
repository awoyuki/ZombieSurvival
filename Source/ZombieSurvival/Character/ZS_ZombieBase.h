#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ZombieSurvival/PoolingSystem/PoolSubsystem.h"
#include "ZombieSurvival/Data/EnemyData.h"
#include "ZS_ZombieBase.generated.h"


UCLASS(Blueprintable)
class ZOMBIESURVIVAL_API AZS_ZombieBase : public ACharacter, public IPoolable
{
	GENERATED_BODY()

public:
	// Sets default values for this empty's properties
	AZS_ZombieBase();

	FTimerHandle DeathTimerHandle;

	//Stored Data
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UEnemyData* EnemyData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int CurrentHealth = 100;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnInitEnemy(UEnemyData* NewData);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void OnEnemyDeath();

	virtual void OnEnemyRemoveFromWorld();

};
