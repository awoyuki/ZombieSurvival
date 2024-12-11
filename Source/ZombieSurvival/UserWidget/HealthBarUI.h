// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Components/ProgressBar.h>
#include "Blueprint/UserWidget.h"
#include "HealthBarUI.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESURVIVAL_API UHealthBarUI : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	void SetBarValuePercent(float const value);

private:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess="true"))
	UProgressBar* HealthValue = nullptr;
	
};
