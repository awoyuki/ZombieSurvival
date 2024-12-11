// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyRangeAbility.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "ZombieSurvival/Character/ZS_ZombieBase.h"
#include <Kismet/GameplayStatics.h>
#include "ZombieSurvival/Item/EnemyBullet.h"
#include "ZombieSurvival/PoolingSystem/PoolSubsystem.h"

void UEnemyRangeAbility::ActiveAbility()
{
    Super::ActiveAbility();
    ZSEnemy->PlayAnimMontage(RangeAttackAnimation, 1, NAME_None);
    if (auto Target = ZSEnemy->GetBlackBoardFromAIController()->GetValueAsObject("TargetActor"))
    {
        TargetActor = Cast<AActor>(Target);
        GetWorld()->GetTimerManager().ClearTimer(RotateTimeHandle);
        GetWorld()->GetTimerManager().SetTimer(RotateTimeHandle, this, &UEnemyRangeAbility::RotateToTarget, 0.01f, true, 0);
    }
}

void UEnemyRangeAbility::EndAbility()
{
    GetWorld()->GetTimerManager().ClearTimer(RotateTimeHandle);
    if (ZSEnemy->GetMesh()->DoesSocketExist("weapon_r_socket"))
    {
        FVector Location = ZSEnemy->GetMesh()->GetSocketLocation("weapon_r_socket");
        AEnemyBullet* Bullet = GetWorld()->GetSubsystem<UPoolSubsystem>()->SpawnFromPool<AEnemyBullet>(BulletClass, Location, ZSEnemy->GetActorRotation());
        Bullet->SetOwner(ZSEnemy);
        Bullet->StartMoving(Location, ZSEnemy->CurrentDamage);
    }
}

void UEnemyRangeAbility::RotateToTarget()
{
    FVector StartLocation = ZSEnemy->GetActorLocation();
    FVector TargetLocation = TargetActor->GetActorLocation();
    FVector Direction = (TargetLocation - StartLocation);

    if (!Direction.IsNearlyZero())
    {
        FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, TargetLocation);
        FRotator CurrentRotation = ZSEnemy->GetActorRotation();

        FRotator NewRotation = FMath::RInterpTo(CurrentRotation, LookAtRotation, GetWorld()->DeltaTimeSeconds, ZSEnemy->EnemyData->MovementSpeed * 5);
        ZSEnemy->SetActorRotation(FRotator(0, NewRotation.Yaw, 0));
        if (FMath::IsNearlyEqual(NewRotation.Yaw, ZSEnemy->GetActorRotation().Yaw, 5))
            GetWorld()->GetTimerManager().ClearTimer(RotateTimeHandle);
    }
}
