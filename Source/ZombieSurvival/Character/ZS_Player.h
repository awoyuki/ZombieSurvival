#pragma once

#include "CoreMinimal.h"
#include "ZombieSurvival/Framework/ZombieSurvivalCharacter.h"
#include "ZS_Player.generated.h"

UCLASS()
class ZOMBIESURVIVAL_API AZS_Player : public AZombieSurvivalCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZS_Player();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
