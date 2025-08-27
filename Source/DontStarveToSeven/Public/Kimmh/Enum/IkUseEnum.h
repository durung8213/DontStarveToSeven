// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IkUseEnum.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EIkUse : uint8
{
	None UMETA(DisplayName = "None"),
	Left UMETA(DisplayName = "Left"),
	Right UMETA(DisplayName = "Right"),
};

class DONTSTARVETOSEVEN_API IkUseEnum
{
public:
	IkUseEnum();
	~IkUseEnum();
};
