// Fill out your copyright notice in the Description page of Project Settings.


#include "Hamkt/Widget/BossStatusWidget.h"

void UBossStatusWidget::UpdateBossName(FString BossName)
{
	if (TextBlock_BossName)
	{
		FString Label = FString::Printf(TEXT("%s"), *BossName);
		TextBlock_BossName->SetText(FText::FromString(Label));
	}
}

void UBossStatusWidget::UpdateBossHPBar(float CurrentHP, float MaxHP)
{
	if (ProgressBar_HPBar)
	{
		float HPPercent = MaxHP > 0 ? CurrentHP / MaxHP : 0.f;
		ProgressBar_HPBar->SetPercent(HPPercent);
		UE_LOG(LogTemp, Warning, TEXT("UpdateBossStatus: CurrentHP=%f, MaxHP=%f, Percent=%f"), CurrentHP, MaxHP, HPPercent);
	}
}
