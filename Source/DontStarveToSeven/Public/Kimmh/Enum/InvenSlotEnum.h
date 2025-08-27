// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InvenSlotEnum.generated.h"

/**
 * 
 */


UENUM(BlueprintType)
enum class EInvenSlot : uint8
{
	InventorySlot UMETA(DisplayName = "InventorySlot"),
	QuickSlot UMETA(DisplayName = "Build"),
	BoxSlot UMETA(DisplayName = "BoxSlot"),
	RootSlot UMETA(DisplayName = "RootSlot"),
	EquipSlot UMETA(DisplayName = "EquipSlot"),
	EmptySlot UMETA(DisplayName = "EmptySlot"),
};

UCLASS()
class DONTSTARVETOSEVEN_API UInvenSlotEnum : public UObject
{
	GENERATED_BODY()
	
};
