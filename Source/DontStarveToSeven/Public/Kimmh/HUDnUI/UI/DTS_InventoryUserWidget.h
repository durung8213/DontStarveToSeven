// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kimmh/HUDnUI/UI/DTS_UserWidget.h"
#include "DTS_InventoryUserWidget.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnGirdInvenSlotHovered, int, Row, int, Col, EInvenSlot, SlotKind, UDTS_InvenSlotUserWidget*, SlotWidget);


UCLASS()
class DONTSTARVETOSEVEN_API UDTS_InventoryUserWidget : public UDTS_UserWidget
{
	GENERATED_BODY()
public:

	UDTS_InventoryUserWidget(const FObjectInitializer& ObjectInitializer);
	UFUNCTION()
	virtual void NativeConstruct() override;

	UFUNCTION()
	void CreateInvenSlot();

	UFUNCTION()
	void ClearAllInvenSlot();

	void LoadInventory();

	UFUNCTION()
	void SlotHovered(int Row, int Col, EInvenSlot SlotKind, UDTS_InvenSlotUserWidget* SlotWidget);



public:
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnGirdInvenSlotHovered OnGirdInvenSlotHovered;

	UPROPERTY(BlueprintReadWrite, Category = "Invnetory")
	int InvnetoryRow = 4;

	UPROPERTY(BlueprintReadWrite, Category = "Invnetory")
	int InvnetoryCol = 4;

private:
	UPROPERTY(meta = (BindWidget))
	class UGridPanel* InvenGrid;

	TArray<class UDTS_InvenSlotUserWidget*> InvenSlotArr;


};
