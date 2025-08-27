// Fill out your copyright notice in the Description page of Project Settings.


#include "Kimmh/HUDnUI/UI/DTS_StateSlotUserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UDTS_StateSlotUserWidget::SetStateSlotValue(float CurrentValue)
{
	SlotCurrentValue = CurrentValue;
	StateSlotValue->SetText(FText::AsNumber(CurrentValue));
	SetProgressBarPercent();
}

void UDTS_StateSlotUserWidget::SetMaxValue(float MaxValue)
{
	SlotMaxValue = MaxValue;
	SetProgressBarPercent();
}

void UDTS_StateSlotUserWidget::SetProgressBarPercent()
{
	SlotProgressBar->SetPercent(SlotCurrentValue / SlotMaxValue);
}


