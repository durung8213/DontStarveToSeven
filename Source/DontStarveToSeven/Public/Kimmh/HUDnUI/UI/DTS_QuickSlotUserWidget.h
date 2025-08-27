// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kimmh/HUDnUI/UI/DTS_UserWidget.h"
#include "DTS_QuickSlotUserWidget.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnQuickInvenSlotHovered, int, Row, int, Col, EInvenSlot, SlotKind, UDTS_InvenSlotUserWidget*, SlotWidget);

UCLASS()
class DONTSTARVETOSEVEN_API UDTS_QuickSlotUserWidget : public UDTS_UserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	virtual void NativeConstruct() override;

	UFUNCTION()
	void CreateQuickSlot();

	UFUNCTION()
	void ClearAllQuickSlot();

	UFUNCTION()
	void LoadQuickSlot();

	UFUNCTION()
	void SlotHovered(int Row, int Col, EInvenSlot SlotKind, UDTS_InvenSlotUserWidget* SlotWidget);

	UFUNCTION()
	void SelectQuickSlot(int SlotNum);

public:
	UPROPERTY()
	FOnQuickInvenSlotHovered OnQuickInvenSlotHovered;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UHorizontalBox* QuickSlotHorizontalBox;

	TArray<class UDTS_InvenSlotUserWidget*> QuickSlotArr;
};
