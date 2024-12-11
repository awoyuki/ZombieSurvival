// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyMelleAbility.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "ZombieSurvival/Character/ZS_ZombieBase.h"
#include <Kismet/GameplayStatics.h>

void UEnemyMelleAbility::ActiveAbility()
{
	Super::ActiveAbility();
	ZSEnemy->PlayAnimMontage(MelleAttackAnimation, 1, NAME_None);
	if(auto Target = ZSEnemy->GetBlackBoardFromAIController()->GetValueAsObject("TargetActor"))
	{
        TargetActor = Cast<AActor>(Target);
        GetWorld()->GetTimerManager().ClearTimer(RotateTimeHandle);
        GetWorld()->GetTimerManager().SetTimer(RotateTimeHandle, this, &UEnemyMelleAbility::RotateToTarget, 0.01f, true, 0);
	}
}

void UEnemyMelleAbility::EndAbility()
{
    GetWorld()->GetTimerManager().ClearTimer(RotateTimeHandle);
	if (ZSEnemy->GetMesh()->DoesSocketExist("weapon_r_socket"))
	{
		TArray<FHitResult> Hits;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(ZSEnemy);
		FCollisionShape ColShape = FCollisionShape::MakeSphere(50);
		FVector Location = ZSEnemy->GetMesh()->GetSocketLocation("weapon_r_socket");
		GetWorld()->SweepMultiByChannel(Hits, Location, Location, FQuat::Identity, ECC_Visibility, ColShape, Params);
		for (auto Hit : Hits)
		{
			if (Hit.bBlockingHit && Hit.GetActor()->ActorHasTag("Player"))
			{
				AController* EnemyController = ZSEnemy->GetInstigatorController();
				UGameplayStatics::ApplyPointDamage(Hit.GetActor(), ZSEnemy->CurrentDamage, Hit.ImpactPoint, Hit, EnemyController, ZSEnemy, UDamageType::StaticClass());
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("Hit Player!"));
				ZSEnemy->SetEnemyState(EEnemyState::Idle);
				return;
			}
			if(Hit.GetComponent())
				UE_LOG(LogClass, Warning, TEXT("Hit Component: %s"), *Hit.GetComponent()->GetName());
		}
	}
}

void UEnemyMelleAbility::RotateToTarget()
{
    FVector StartLocation = ZSEnemy->GetActorLocation();
    FVector TargetLocation = TargetActor->GetActorLocation();
    FVector Direction = (TargetLocation - StartLocation);

    if (!Direction.IsNearlyZero())
    {
        FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, TargetLocation);
        FRotator CurrentRotation = ZSEnemy->GetActorRotation();

        FRotator NewRotation = FMath::RInterpTo(CurrentRotation, LookAtRotation, GetWorld()->DeltaTimeSeconds ,ZSEnemy->EnemyData->MovementSpeed * 5);
        ZSEnemy->SetActorRotation(FRotator(0 , NewRotation.Yaw, 0));
		if (FMath::IsNearlyEqual(NewRotation.Yaw, ZSEnemy->GetActorRotation().Yaw, 5))
			GetWorld()->GetTimerManager().ClearTimer(RotateTimeHandle);
    }
}
