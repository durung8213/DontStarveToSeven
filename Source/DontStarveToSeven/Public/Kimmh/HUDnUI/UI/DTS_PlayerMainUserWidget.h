// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kimmh/HUDnUI/UI/DTS_UserWidget.h"
#include "Kimmh/HUDnUI/UI/DTS_InvenSlotUserWidget.h"
#include "Kimmh/Enum/InvenSlotEnum.h"
#include "DTS_PlayerMainUserWidget.generated.h"

/**
 * 
 */
USTRUCT(Atomic, BlueprintType)
struct DONTSTARVETOSEVEN_API FItemSlot {
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Row;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Col;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EInvenSlot SlotKind;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDTS_InvenSlotUserWidget* SlotWidget;
};


UCLASS()
class DONTSTARVETOSEVEN_API UDTS_PlayerMainUserWidget : public UDTS_UserWidget
{
	GENERATED_BODY()
	
public:
	UDTS_PlayerMainUserWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

	class UDTS_InventoryUserWidget* CreateInventoryUserWidget();

	void ReLoadInventory();

	void ReloadQuickSlot();

	void SetTempSlot();


	void ClearTempSlot();

	void TempSlotVisible(bool bIsVisible);

	void SetItemInfoVisible(bool bIsVisible);

	void UpdateTempSlotPositionToMouse();

	void UpdateItemInfoPositionToMouse();

	UFUNCTION()
	void OnSlotHovered(int Row, int Col, EInvenSlot SlotKind, UDTS_InvenSlotUserWidget* SlotWidget);
	
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
	UPROPERTY(meta = (BindWidget))
	class UBorder* InventoryBorder;

	UPROPERTY()
	class UDTS_InventoryUserWidget* DTS_InvnetoryUserWidget;

	UPROPERTY(meta = (BindWidget))
	class UDTS_InventoryTempSlotUserWidget* WBP_InvenTempSlot;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, EditAnywhere)
	class UDTS_QuickSlotUserWidget* WBP_QuickSlot;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, EditAnywhere)
	class UDTS_EmptyUserWidget* WBP_Empty;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, EditAnywhere)
	class UDTS_ItemInfoUserWidget* WBP_ItemInfo;

	UPROPERTY();
	bool bUpdateTempSlotPositionToMouse;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UDTS_InventoryUserWidget> InvnetoyWBPClass;

private:
	FItemSlot CurrentSlot;

	FItemSlot OldSlot;

	bool bIsCanInteraction;

	bool bIsCanOpenItemInfo;
};
