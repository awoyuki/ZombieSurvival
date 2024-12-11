// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletBase.h"
#include "Sound/SoundCue.h"
#include "ZombieSurvival/PoolingSystem/PoolSubsystem.h"

// Sets default values
ABulletBase::ABulletBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerSphere"));
	SetRootComponent(TriggerSphere);
	TriggerSphere->SetGenerateOverlapEvents(false);
	TriggerSphere->SetNotifyRigidBodyCollision(true);
	TriggerSphere->SetCollisionProfileName("Projectile");
	TriggerSphere->OnComponentHit.AddDynamic(this, &ABulletBase::OnBulletHit);

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	BulletMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BulletMesh->SetupAttachment(TriggerSphere);


	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileComponent->SetAutoActivate(false);
	ProjectileComponent->SetActive(false);
	ProjectileComponent->InitialSpeed = 0;
	ProjectileComponent->MaxSpeed = 0;
}

// Called when the game starts or when spawned
void ABulletBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABulletBase::UpdateBulletVelocity(FVector NewVelocity)
{
	ProjectileComponent->Velocity = GetActorTransform().TransformVector(NewVelocity);
}

// Called every frame
void ABulletBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABulletBase::StartMoving(FVector Target, float Damage)
{
	BulletDamage = Damage;
	ProjectileComponent->SetActive(true);
	ProjectileComponent->InitialSpeed = BulletSpeed;
	ProjectileComponent->MaxSpeed = BulletSpeed*10;
	UpdateBulletVelocity(BulletVeclocity);
	ProjectileComponent->UpdateComponentVelocity();
}

void ABulletBase::OnBulletHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	EndMove(Hit);
}


void ABulletBase::EndMove(const FHitResult& Hit)
{
	const UObject* WorldContextObject = GetWorld();
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactFX, GetActorTransform(), true, EPSCPoolMethod::AutoRelease);
	UGameplayStatics::PlaySoundAtLocation(WorldContextObject, ImpactSFX, GetActorLocation());

	ProjectileComponent->SetActive(false);
	GetWorld()->GetSubsystem<UPoolSubsystem>()->ReturnToPool(this);
}

