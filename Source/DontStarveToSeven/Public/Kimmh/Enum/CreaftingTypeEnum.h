// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CreaftingTypeEnum.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ECreaftingType : uint8
{
	Item UMETA(DisplayName = "Item"),
	Build UMETA(DisplayName = "Build")
};

class DONTSTARVETOSEVEN_API CreaftingTypeEnum
{
public:
	CreaftingTypeEnum();
	~CreaftingTypeEnum();
};
