// Fill out your copyright notice in the Description page of Project Settings.


#include "Kimmh/HUDnUI/UI/DTS_InventoryTempSlotUserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UDTS_InventoryTempSlotUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UDTS_InventoryTempSlotUserWidget::SetItemImage(UTexture2D* ItemIcon)
{
	if (ItemIcon) {
		ItemImage->SetVisibility(ESlateVisibility::Visible);
		ItemImage->SetBrushFromTexture(ItemIcon);
	}
}

void UDTS_InventoryTempSlotUserWidget::SetItemCount(int ItemCount)
{
	if (ItemCount) {
		ItemCountText->SetVisibility(ESlateVisibility::Visible);
		ItemCountText->SetText(FText::AsNumber(ItemCount));
	}
}

void UDTS_InventoryTempSlotUserWidget::ClearItem()
{
	ItemImage->SetVisibility(ESlateVisibility::Collapsed);
	ItemCountText->SetVisibility(ESlateVisibility::Collapsed);
}

void UDTS_InventoryTempSlotUserWidget::OnVisible()
{
	this->SetVisibility(ESlateVisibility::Visible);
}

void UDTS_InventoryTempSlotUserWidget::OnCollapsed()
{
	this->SetVisibility(ESlateVisibility::Collapsed);
}
