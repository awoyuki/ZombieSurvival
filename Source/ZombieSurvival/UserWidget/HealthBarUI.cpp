// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBarUI.h"

void UHealthBarUI::NativeConstruct()
{
	Super::NativeConstruct();
}

void UHealthBarUI::SetBarValuePercent(float const value)
{
	HealthValue->SetPercent(value);
}
