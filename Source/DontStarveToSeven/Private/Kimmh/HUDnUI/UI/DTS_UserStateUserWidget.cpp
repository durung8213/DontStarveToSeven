// Fill out your copyright notice in the Description page of Project Settings.


#include "Kimmh/HUDnUI/UI/DTS_UserStateUserWidget.h"
#include "Kimmh/HUDnUI/UI/DTS_StateSlotUserWidget.h"
#include "Kimmh/Character/PlayerState/DTS_BasePlayerState.h"
#include "Kimmh/HUDnUI/UI/DTS_QuickSlotUserWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kimmh/HUDnUI/UI/DTS_BossHpBarUserWidget.h"


void UDTS_UserStateUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (GetOwningPlayer()) {
		FString Message = FString::Printf(TEXT("GetOwningPlayer Call Success"));
		UKismetSystemLibrary::PrintString(this, Message, true, true, FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));
	}
	else {
		FString Message = FString::Printf(TEXT("GetOwningPlayer Call Fail"));
		UKismetSystemLibrary::PrintString(this, Message, true, true, FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));
	}

	Init();

	WBP_BossHpBar->SetBossHpBarVisibility(false);
}

void UDTS_UserStateUserWidget::Init()
{
	FString Message = FString::Printf(TEXT("Init Start"));
	UKismetSystemLibrary::PrintString(this, Message);

	GetWorld()->GetTimerManager().ClearTimer(InitHandle);

	if (GetOwningPlayerState()) {
		ADTS_BasePlayerState* CurrentPlayerState = Cast<ADTS_BasePlayerState>(GetOwningPlayerState());
		if (CurrentPlayerState) {
			SetMaxHp(CurrentPlayerState->GetMaxHp());
			SetCurrentHp(CurrentPlayerState->GetCurrentHp());
			SetMaxMp(CurrentPlayerState->GetMaxMp());
			SetCurrentMp(CurrentPlayerState->GetCurrentMp());
			SetMaxHunger(CurrentPlayerState->GetMaxHunger());
			SetCurrentHunger(CurrentPlayerState->GetCurrentHunger());
			Message = FString::Printf(TEXT("CurrentPlayerState Cast Success"));
			UKismetSystemLibrary::PrintString(this, Message, true, true, FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));
		}
		else {
			Message = FString::Printf(TEXT("CurrentPlayerState Cast Fail"));
			UKismetSystemLibrary::PrintString(this, Message, true, true, FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));

			GetWorld()->GetTimerManager().SetTimer(
				InitHandle,
				this,
				&UDTS_UserStateUserWidget::Init,
				0.3f,
				false
			);
		}
	}
	else {
		Message = FString::Printf(TEXT("GetOwningPlayerState Call Fail"));
		UKismetSystemLibrary::PrintString(this, Message, true, true, FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));

		Message = FString::Printf(TEXT("CurrentPlayerState Cast Fail"));
		UKismetSystemLibrary::PrintString(this, Message, true, true, FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));

		GetWorld()->GetTimerManager().SetTimer(
			InitHandle,
			this,
			&UDTS_UserStateUserWidget::Init,
			1.f,
			false
		);
	}

}

void UDTS_UserStateUserWidget::SetMaxHp(float CurrentHp)
{
	if (WBP_HpStateSlot) {
		WBP_HpStateSlot->SetMaxValue(CurrentHp);

	}
}

void UDTS_UserStateUserWidget::SetCurrentHp(float CurrentHp)
{
	if (IsValid(this)) {
		if (WBP_HpStateSlot) {
			WBP_HpStateSlot->SetStateSlotValue(CurrentHp);

		}
	}
	
}

void UDTS_UserStateUserWidget::SetMaxMp(float MaxMp)
{

	WBP_MpStateSlot->SetMaxValue(MaxMp);
}

void UDTS_UserStateUserWidget::SetCurrentMp(float CurrentMp)
{
	WBP_MpStateSlot->SetStateSlotValue(CurrentMp);
}

void UDTS_UserStateUserWidget::SetMaxHunger(float MaxHunger)
{
	WBP_HungerStateSlot->SetMaxValue(MaxHunger);
}

void UDTS_UserStateUserWidget::SetCurrentHunger(float CurrentHunger)
{
	WBP_HungerStateSlot->SetStateSlotValue(CurrentHunger);
}

void UDTS_UserStateUserWidget::SelectQuickSlot(int SlotNum)
{
	if (WBP_QuickSlot) {
		WBP_QuickSlot->SelectQuickSlot(SlotNum);
	}
	else {
		FString Message;
		Message = FString::Printf(TEXT("WBP_QuickSlot is Null"));
		UKismetSystemLibrary::PrintString(this, Message, true, true, FLinearColor(1.f, 0.f, 0.f), 2.f);
	}
}

void UDTS_UserStateUserWidget::ReLoadQuickSlot()
{
	WBP_QuickSlot->LoadQuickSlot();
}

void UDTS_UserStateUserWidget::SetBossHpBarVisibility(bool bIsVisbile)
{
	WBP_BossHpBar->SetBossHpBarVisibility(bIsVisbile);
}

void UDTS_UserStateUserWidget::SetBossHpBarPercent(float InCurrentHealth, float InMaxHealth)
{
	WBP_BossHpBar->SetBossHpBarPercent(InCurrentHealth, InMaxHealth);
}

void UDTS_UserStateUserWidget::SetBossName(FText BossName)
{
	WBP_BossHpBar->SetBossName(BossName);
}
