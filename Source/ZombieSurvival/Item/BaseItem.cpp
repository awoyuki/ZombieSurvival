#include "BaseItem.h"

// Sets default values
ABaseItem::ABaseItem()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Item Components
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(SceneRoot);
	MeshComponent->AddRelativeLocation(FVector(0, 0, 100));
	MeshComponent->SetCollisionProfileName(TEXT("NoCollision"));

	TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerSphere"));
	TriggerSphere->SetupAttachment(SceneRoot);
	TriggerSphere->AddRelativeLocation(FVector(0, 0, 100));
	TriggerSphere->SetGenerateOverlapEvents(true);
	TriggerSphere->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
	TriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &ABaseItem::OnSphereBeginOverlapEvent);


}

// Called when the game starts or when spawned
void ABaseItem::BeginPlay()
{
	Super::BeginPlay();
	PoolSubsystem = GetWorld()->GetSubsystem<UPoolSubsystem>();
}

// Called every frame
void ABaseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseItem::OnSphereBeginOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(PoolSubsystem)) {
		return;
	}

	if(OtherActor->ActorHasTag(TEXT("Player")))
		OnPlayerOverlap(OtherActor);

}

void ABaseItem::OnPlayerOverlap(AActor* PlayerActor)
{
	PoolSubsystem->ReturnToPool(this);
}

void ABaseItem::OnItemSpawn()
{
}

