// Fill out your copyright notice in the Description page of Project Settings.


#include "GrenadeBullet.h"
#include "ZombieSurvival/PoolingSystem/PoolSubsystem.h"
#include "Sound/SoundCue.h"
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/GameplayStatics.h>



AGrenadeBullet::AGrenadeBullet()
{
}

FVector AGrenadeBullet::CalculateProjectileVelocity(FVector StartLocation, FVector Target)
{
	FVector Delta = Target - StartLocation;

	FVector HorizontalDelta = FVector(Delta.X, Delta.Y, 0.0f);
	float HorizontalDistance = HorizontalDelta.Size();
	float VerticalDelta = Delta.Z;

	float Time = HorizontalDistance / BulletSpeed;

	FVector HorizontalVelocity = HorizontalDelta / Time;

	float VerticalVelocity = (VerticalDelta + (0.5f * 980 * FMath::Square(Time))) / Time;

	return HorizontalVelocity + FVector(0.0f, 0.0f, VerticalVelocity);
}

void AGrenadeBullet::StartMoving(FVector Target, float Damage)
{
	Super::StartMoving(Target, Damage);
	TargetLocation = Target;
	FVector InitialVelocity = CalculateProjectileVelocity(GetActorLocation(), Target);
	if (ProjectileComponent)
	{
		ProjectileComponent->Velocity = InitialVelocity;
		ProjectileComponent->UpdateComponentVelocity();
	}
}

void AGrenadeBullet::EndMove(const FHitResult& Hit)
{
	const UObject* WorldContextObject = GetWorld();
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactFX, GetActorTransform(), true, EPSCPoolMethod::AutoRelease);
	UGameplayStatics::PlaySoundAtLocation(WorldContextObject, ImpactSFX, GetActorLocation());

	const TArray<AActor*> IgnoreActor = { Owner };

	UGameplayStatics::ApplyRadialDamage(WorldContextObject, BulletDamage, GetActorLocation(), BulletRadius, nullptr, IgnoreActor, Owner, Owner->GetInstigatorController(), true, ECC_Camera);
	Super::EndMove(Hit);
}