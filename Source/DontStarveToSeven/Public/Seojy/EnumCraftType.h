#pragma once

#include "CoreMinimal.h"
#include "EnumCraftType.generated.h"

UENUM(BlueprintType)
enum class ECraftType : uint8
{
    Shelter       UMETA(DisplayName = "Shelter"),
    AttackTurret  UMETA(DisplayName = "AttackTurret"),
    HealTurret    UMETA(DisplayName = "HealTurret"),
    BonFire       UMETA(DisplayName = "BonFire"),
    CrockPot      UMETA(DisplayName = "CrockPot"),
    WoodFence     UMETA(DisplayName = "WoodFence"),
    None          UMETA(DisplayName = "None")
};

UENUM(BlueprintType)
enum class ECraftMode : uint8
{
    Ghost   UMETA(DisplayName = "Ghost"),
    Actual  UMETA(DisplayName = "Actual")
};