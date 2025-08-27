// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kimmh/HUDnUI/UI/DTS_UserWidget.h"
#include "DTS_InventoryTempSlotUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class DONTSTARVETOSEVEN_API UDTS_InventoryTempSlotUserWidget : public UDTS_UserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "InvneTempSlot")
	void SetItemImage(UTexture2D* ItemIcon);

	UFUNCTION(BlueprintCallable, Category = "InvneTempSlot")
	void SetItemCount(int ItemCount);

	UFUNCTION(BlueprintCallable, Category = "InvneTempSlot")
	void ClearItem();

	UFUNCTION(BlueprintCallable, Category = "InvneTempSlot")
	void OnVisible();

	UFUNCTION(BlueprintCallable, Category = "InvneTempSlot")
	void OnCollapsed();

public:

	UPROPERTY(meta = (BindWidget))
	class UImage* ItemImage;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemCountText;


	
	
};
