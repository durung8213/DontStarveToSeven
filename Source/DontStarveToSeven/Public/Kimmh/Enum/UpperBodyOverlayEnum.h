// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UpperBodyOverlayEnum.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EUpperOverlay : uint8
{
	TwoHandSword UMETA(DisplayName = "TwoHandWeapon"),
	OneHandSword UMETA(DisplayName = "OneHandSword"),
	Spear UMETA(DisplayName = "Spear"),
	Bow UMETA(DisplayName = "Bow"),
	Pistol UMETA(DisplayName = "Pistol"),
	Rifle UMETA(DisplayName = "Rifle"),
	Normal UMETA(DisplayName = "Normal")
};

class DONTSTARVETOSEVEN_API UpperBodyOverlayEnum
{
public:
	UpperBodyOverlayEnum();
	~UpperBodyOverlayEnum();
};
