// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletBase.h"
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/GameplayStatics.h>

// Sets default values
ABulletBase::ABulletBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	SetRootComponent(BulletMesh);
}

// Called when the game starts or when spawned
void ABulletBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABulletBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABulletBase::StartMoving(FVector Target)
{
	TargetLocation = Target;
	TargetDistance = FVector::DistSquaredXY(GetActorLocation(), Target);
	StartHeight = GetActorLocation().Z;
	GetWorldTimerManager().SetTimer(MoveTimeHandle, this, &ABulletBase::Moving, GetWorld()->GetDeltaSeconds(), true, 0);
}

void ABulletBase::Moving()
{
	FVector CurrentLocation = GetActorLocation();
	FRotator CurrentRotation = GetActorRotation();
	FRotator TargetRot = UKismetMathLibrary::MakeRotFromX(TargetLocation - CurrentLocation);

	CurrentLocation = FMath::Lerp(CurrentLocation, TargetLocation, BulletSpeed * GetWorld()->GetDeltaSeconds());
	CurrentRotation = FMath::Lerp(CurrentRotation, TargetRot, BulletSpeed * GetWorld()->GetDeltaSeconds());

	float CurrentDistance = FVector::DistSquaredXY(GetActorLocation(), TargetLocation);
	CurrentLocation.Z = FMath::Lerp(TargetLocation.Z, StartHeight + 20.0f, FMath::Sin(PI * CurrentDistance / TargetDistance));

	SetActorLocation(CurrentLocation);
	SetActorRotation(CurrentRotation);

	if (CurrentDistance < 0.1f) 
	{
		GetWorldTimerManager().ClearTimer(MoveTimeHandle);
		EndMove();
	}
}

void ABulletBase::EndMove()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("Sphere Damage Applied!"));
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactFX, GetActorTransform(), true, EPSCPoolMethod::AutoRelease);
}

