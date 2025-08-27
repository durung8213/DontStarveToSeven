// Fill out your copyright notice in the Description page of Project Settings.


#include "Kimmh/HUDnUI/UI/DTS_EmptyUserWidget.h"
#include "Kimmh/Enum/InvenSlotEnum.h"

void UDTS_EmptyUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UDTS_EmptyUserWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);
	if (IsHovered()) {
		OnEmptySlotHovered.Broadcast(-1, -1, EInvenSlot::EmptySlot, nullptr);
	}
}

