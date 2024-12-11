#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "ZombieSurvival/PoolingSystem/PoolSubsystem.h"
#include "BaseItem.generated.h"

UCLASS()
class ZOMBIESURVIVAL_API ABaseItem : public AActor, public IPoolable
{
	GENERATED_BODY()
	
protected:
	UPoolSubsystem* PoolSubsystem;

public:
	// Sets default values for this actor's properties
	ABaseItem();

	// Scene Root
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* SceneRoot;

	// Item visualize
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* TriggerSphere;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnItemSpawn();

	UFUNCTION()
	void OnSphereBeginOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnPlayerOverlap(AActor* PlayerActor);
	virtual void OnPlayerOverlap_Implementation(AActor* PlayerActor);
};
