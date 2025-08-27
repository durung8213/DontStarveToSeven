// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CreaftingKindEnum.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ECreaftingKind : uint8
{
	Favourite UMETA(DisplayName = "Favourite"),
	Item UMETA(DisplayName = "Item"),
	Common UMETA(DisplayName = "Common"),
	Tool UMETA(DisplayName = "Tool"),
	Weapon UMETA(DisplayName = "Weapon"),
	Turret UMETA(DisplayName = "Turret"),
	Structures UMETA(DisplayName = "Structures")
};

class DONTSTARVETOSEVEN_API CreaftingKindEnum
{
public:
	CreaftingKindEnum();
	~CreaftingKindEnum();
};
