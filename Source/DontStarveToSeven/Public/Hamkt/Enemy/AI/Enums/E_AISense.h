#pragma once

#include "CoreMinimal.h"
#include "E_AISense.generated.h"

UENUM(BlueprintType)
enum class E_AISense : uint8
{
	None		UMETA(DisplayName = "None"),
	Sight		UMETA(DisplayName = "Sight"),
	Hearing		UMETA(DisplayName = "Hearing"),
	Damage		UMETA(DisplayName = "Damage")
};