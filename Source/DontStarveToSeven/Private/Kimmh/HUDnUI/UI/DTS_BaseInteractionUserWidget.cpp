// Fill out your copyright notice in the Description page of Project Settings.


#include "Kimmh/HUDnUI/UI/DTS_BaseInteractionUserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

#include "TimerManager.h"

void UDTS_BaseInteractionUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetPercent(0);
	SetRoundProgressBarCollapsed();

}

void UDTS_BaseInteractionUserWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);
	if (bProgressBarActive) {
		ElapsedTime += DeltaTime;
		float Percent = FMath::Clamp(ElapsedTime / DurationTime, 0.0f, 1.0f);

		SetPercent(Percent);

		if (Percent >= 1) {
			if (!bIsProgressComplete) {
				bIsProgressComplete = true;
				ProgressComplete();
			}
		}
	}
}

void UDTS_BaseInteractionUserWidget::SetVisibilityWidget()
{
	SetVisibility(ESlateVisibility::Visible);
	GetWorld()->GetTimerManager().ClearTimer(VisableTimer);
	GetWorld()->GetTimerManager().SetTimer(VisableTimer, this, &UDTS_BaseInteractionUserWidget::SetCollapsedWidget, 0.5f, false);
}

void UDTS_BaseInteractionUserWidget::SetCollapsedWidget()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UDTS_BaseInteractionUserWidget::SetInteractionText(FText InteractionText)
{
	InteractionTextBlock->SetText(InteractionText);
}

void UDTS_BaseInteractionUserWidget::SetInputKeyVisible()
{
	InputKey->SetVisibility(ESlateVisibility::Visible);
}

void UDTS_BaseInteractionUserWidget::SetInputKeyCollapsed()
{
	InputKey->SetVisibility(ESlateVisibility::Collapsed);
}

void UDTS_BaseInteractionUserWidget::SetRoundPorgressBarVisible(float Duraction)
{
	RoundProgressBar->SetVisibility(ESlateVisibility::Visible);
	DurationTime = Duraction;
	ElapsedTime = 0;
	SetPercent(0);
	bProgressBarActive = true;
}

void UDTS_BaseInteractionUserWidget::SetRoundProgressBarCollapsed()
{
	RoundProgressBar->SetVisibility(ESlateVisibility::Collapsed);
	bProgressBarActive = false;
}

void UDTS_BaseInteractionUserWidget::ProgressComplete()
{

}



