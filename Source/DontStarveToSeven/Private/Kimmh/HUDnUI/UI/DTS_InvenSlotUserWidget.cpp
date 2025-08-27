// Fill out your copyright notice in the Description page of Project Settings.


#include "Kimmh/HUDnUI/UI/DTS_InvenSlotUserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kimmh/Enum/InvenSlotEnum.h"
#include "Kismet/KismetSystemLibrary.h"

void UDTS_InvenSlotUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UDTS_InvenSlotUserWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);
	if (IsHovered()) {
		OnInvenSlotHovered.Broadcast(Row, Col, SlotKind, this);
	}
}


void UDTS_InvenSlotUserWidget::SetItemImage(UTexture2D* ItemIcon)
{
	if (ItemIcon) {
		ItemImage->SetVisibility(ESlateVisibility::Visible);
		ItemImage->SetBrushFromTexture(ItemIcon);
		ItemIconTexture = ItemIcon;
	}
}

void UDTS_InvenSlotUserWidget::SetItemCount(int ItemCount)
{
	ItemCountText->SetVisibility(ESlateVisibility::Visible);
	ItemCountText->SetText(FText::AsNumber(ItemCount));
	ItemCountValue = ItemCount;
}

void UDTS_InvenSlotUserWidget::ClearItem() 
{
	bItemSet = false;
	ItemImage->SetVisibility(ESlateVisibility::Collapsed);
	ItemCountText->SetVisibility(ESlateVisibility::Collapsed);
}

void UDTS_InvenSlotUserWidget::PrintRowCol()
{
	FString Message = FString::Printf(TEXT("Row: %d, Col: %d"), Row,Col);
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, Message); // 화면에 표시
}

void UDTS_InvenSlotUserWidget::QuickSlotSelect()
{
	InvenSlotBackground->SetColorAndOpacity(FLinearColor(0.005205f, 0.027914f, 0.062500f, 0.794000f));
}

void UDTS_InvenSlotUserWidget::QuickSlotNotSelect()
{
	InvenSlotBackground->SetColorAndOpacity(FLinearColor(0.036458f, 0.036458f, 0.036458f, 0.794000f));
}







