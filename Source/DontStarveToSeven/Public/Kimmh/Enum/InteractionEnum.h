// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionEnum.generated.h"
/**
 * 
 */


UENUM(BlueprintType)
enum class EInteraction : uint8
{
	Item UMETA(DisplayName = "Item"),
	Tree UMETA(DisplayName = "Tree"),
	Rock UMETA(DisplayName = "Rock"),
	Bush UMETA(DisplayName = "Bush"),
	Fire UMETA(DisplayName = "Fire"),

};

class DONTSTARVETOSEVEN_API InteractionEnum
{
public:
	InteractionEnum();
	~InteractionEnum();
};
