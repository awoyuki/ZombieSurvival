// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBullet.h"

void AEnemyBullet::EndMove(const FHitResult& Hit)
{
	if (Hit.GetActor()->ActorHasTag("Player"))
	{
		UGameplayStatics::ApplyPointDamage(Hit.GetActor(), BulletDamage, GetActorLocation(), Hit, Owner->GetInstigatorController(), Owner, UDamageType::StaticClass());
	}
	Super::EndMove(Hit);
}
