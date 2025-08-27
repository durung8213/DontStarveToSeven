// Fill out your copyright notice in the Description page of Project Settings.


#include "Kimmh/HUDnUI/UI/DTS_RequireItemSlotUserWidget.h"
#include "Components/TextBlock.h"

void UDTS_RequireItemSlotUserWidget::SetItemAmount(int ItemAmount)
{
	ItemAmountText->SetText(FText::AsNumber(ItemAmount));
}

void UDTS_RequireItemSlotUserWidget::SetItemName(FText ItemName)
{
	ItemNameText->SetText(ItemName);
	ResourceItemName = ItemName.ToString();
}

void UDTS_RequireItemSlotUserWidget::SetItemCountTotal(int ItemTotal)
{
	ItemCountTotalText->SetText(FText::AsNumber(ItemTotal));
	TotalRequireItemCount = ItemTotal;
}

void UDTS_RequireItemSlotUserWidget::SetItemHave(int ItemHave)
{
	ItemHaveText->SetText(FText::AsNumber(ItemHave));
}
