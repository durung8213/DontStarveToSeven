// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kimmh/HUDnUI/UI/DTS_UserWidget.h"
#include "DTS_EmptyUserWidget.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnEmptySlotHovered, int, Row, int, Col, EInvenSlot, SlotKind, UDTS_InvenSlotUserWidget*, SlotWidget);
UCLASS()
class DONTSTARVETOSEVEN_API UDTS_EmptyUserWidget : public UDTS_UserWidget
{
	GENERATED_BODY()
	

public:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

	FOnEmptySlotHovered OnEmptySlotHovered;
};
