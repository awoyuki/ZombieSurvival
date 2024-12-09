#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ZombieSurvival/PoolingSystem/PoolSubsystem.h"
#include "ZS_ZombieBase.generated.h"

UCLASS(Blueprintable)
class ZOMBIESURVIVAL_API AZS_ZombieBase : public ACharacter, public IPoolable
{
	GENERATED_BODY()

public:
	// Sets default values for this empty's properties
	AZS_ZombieBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
