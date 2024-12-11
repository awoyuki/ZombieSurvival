// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyStunAbility.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "ZombieSurvival/Character/ZS_ZombieBase.h"
#include <Kismet/GameplayStatics.h>
#include "ZombieSurvival/Item/EnemyBullet.h"
#include "ZombieSurvival/PoolingSystem/PoolSubsystem.h"
#include <ZombieSurvival/Character/ZS_Player.h>


void UEnemyStunAbility::ActiveAbility()
{
    ZSEnemy->PlayAnimMontage(StunAttackAnimation, 1, NAME_None);
    if (auto Target = ZSEnemy->GetBlackBoardFromAIController()->GetValueAsObject("TargetActor"))
    {
        TargetActor = Cast<AActor>(Target);
    }
}

void UEnemyStunAbility::EndAbility()
{
	TArray<FHitResult> Hits;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(ZSEnemy);
	FCollisionShape ColShape = FCollisionShape::MakeSphere(StunRadius);
	FVector Location = ZSEnemy->GetActorLocation();
	GetWorld()->SweepMultiByChannel(Hits, Location, Location, FQuat::Identity, ECC_Visibility, ColShape, Params);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, StunAttackFX, Location, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	for (const auto Hit : Hits)
	{
		if (Hit.bBlockingHit && Hit.GetActor()->ActorHasTag("Player"))
		{
			AController* EnemyController = ZSEnemy->GetInstigatorController();
			UGameplayStatics::ApplyDamage(Hit.GetActor(), ZSEnemy->CurrentDamage, EnemyController, ZSEnemy, UDamageType::StaticClass());
			ZSEnemy->SetEnemyState(EEnemyState::Idle);
			if (AZS_Player* Player = Cast<AZS_Player>(Hit.GetActor()))
			{
				Player->OnPlayerStunned(StunTime);
			}
			return;
		}
		if (Hit.GetComponent())
			UE_LOG(LogClass, Warning, TEXT("Hit Component: %s"), *Hit.GetComponent()->GetName());
	}
}
