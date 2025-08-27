// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kimmh/HUDnUI/UI/DTS_UserWidget.h"
#include "DTS_StateSlotUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class DONTSTARVETOSEVEN_API UDTS_StateSlotUserWidget : public UDTS_UserWidget
{
	GENERATED_BODY()
	
public:
	void SetStateSlotValue(float CurrentValue);

	void SetMaxValue(float MaxValue);

	void SetProgressBarPercent();

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* StateSlotValue;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* SlotProgressBar;

	float SlotCurrentValue = 0;

	float SlotMaxValue = 0;
	
};
