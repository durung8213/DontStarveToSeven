// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kimmh/HUDnUI/UI/DTS_UserWidget.h"
#include "Blueprint/UserWidget.h"
#include "DTS_InvenSlotUserWidget.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnInvenSlotHovered, int, Row, int, Col, EInvenSlot, SlotKind, UDTS_InvenSlotUserWidget*, SlotWidget);

UCLASS()
class DONTSTARVETOSEVEN_API UDTS_InvenSlotUserWidget : public UDTS_UserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "InvneSlot")
	void SetItemImage(UTexture2D* ItemIcon);

	UFUNCTION(BlueprintCallable, Category = "InvneSlot")
	void SetItemCount(int ItemCount);

	UFUNCTION(BlueprintCallable, Category = "InvneSlot")
	void ClearItem();

	UFUNCTION(BlueprintCallable, Category = "InvneSlot")
	void PrintRowCol();

	UFUNCTION(BlueprintCallable, Category = "InvneQuickSlot")
	void QuickSlotSelect();

	UFUNCTION(BlueprintCallable, Category = "InvneQuickSlot")
	void QuickSlotNotSelect();

public:
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnInvenSlotHovered OnInvenSlotHovered;

	UPROPERTY(meta = (BindWidget))
	class UImage* InvenSlotBackground;

	UPROPERTY(meta=(BindWidget))
	class UImage* ItemImage;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemCountText;

	UPROPERTY(BlueprintReadOnly, Category = "InvneSlot")
	class UTexture2D* ItemIconTexture;

	UPROPERTY(BlueprintReadOnly, Category = "InvneSlot")
	int ItemCountValue;

	UPROPERTY(BlueprintReadOnly, Category = "InvneSlot")
	int Row;

	UPROPERTY(BlueprintReadOnly, Category = "InvneSlot")
	int Col;

	UPROPERTY(BlueprintReadOnly, Category = "InvneSlot")
	FString ItemName;

	bool bItemSet = false;
	
	EInvenSlot SlotKind;

};
