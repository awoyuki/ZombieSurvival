// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieSurvivalGameState.h"
#include <Kismet/GameplayStatics.h>

void AZombieSurvivalGameState::BeginPlay()
{
	SetGameStatus_Implementation(EGameStatus::Inprogress);
}

AZombieSurvivalGameState::AZombieSurvivalGameState()
{
}

void AZombieSurvivalGameState::GetGameStatus_Implementation(EGameStatus &GameStatus)
{
	GameStatus = CurrentGameStatus;
}

void AZombieSurvivalGameState::SetGameStatus_Implementation(EGameStatus NewStatus)
{
	CurrentGameStatus = NewStatus;
	OnGameStatusChanged.Broadcast(NewStatus);
}

void AZombieSurvivalGameState::LoadNextZone_Implementation()
{
	if (CurrentGameStatus == EGameStatus::Lose) return;

	CurrentLevelIndex++;
	if (CurrentLevelIndex > MaxLevelCount)
		SetGameStatus_Implementation(EGameStatus::Win);

	FLatentActionInfo LatentInfo;

	FString LevelName = LevelPrefix;
	LevelName.Append(FString::FromInt(CurrentLevelIndex));
	UE_LOG(LogTemp, Warning, TEXT("LevelLoading %s"), *LevelName);
	UGameplayStatics::LoadStreamLevel(this, FName(*LevelName), true, true, LatentInfo);

}




