// Fill out your copyright notice in the Description page of Project Settings.


#include "Kimmh/HUDnUI/UI/DTS_BossHpBarUserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UDTS_BossHpBarUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UDTS_BossHpBarUserWidget::SetBossHpBarVisibility(bool bIsVisbile)
{
	if (bIsVisbile) {
		SetVisibility(ESlateVisibility::Visible);
	}
	else {
		SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UDTS_BossHpBarUserWidget::SetBossName(FText BossName)
{
	BossNameTextBlock->SetText(BossName);
}

void UDTS_BossHpBarUserWidget::SetBossName(FString BossName)
{
	BossNameTextBlock->SetText(FText::FromString(*BossName));
}


void UDTS_BossHpBarUserWidget::SetBossHpBarPercent(float InCurrentHealth, float InMaxHealth)
{
	BossHpBar->SetPercent(InCurrentHealth/ InMaxHealth);
}

