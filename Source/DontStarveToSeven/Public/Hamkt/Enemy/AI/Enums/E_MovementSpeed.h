// Fill out your copyright notice in the Description page of Project Settings.
// .cpp 파일 제거 필요
#pragma once

#include "CoreMinimal.h"
#include "E_MovementSpeed.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class E_MovementSpeed : uint8
{
	Idle		UMETA(DisplayName = "Idle"),
	Walking		UMETA(DisplayName = "Walking"),
	Jogging		UMETA(DisplayName = "Jogging"),
	Sprinting	UMETA(DisplayName = "Sprinting")
};