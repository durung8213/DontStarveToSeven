#pragma once

#include "CoreMinimal.h"
#include "Seojy/EnumCraftType.h"
#include "BuildingData.generated.h"

USTRUCT(BlueprintType)

// 한 건물이 차지하게 될 가로, 세로 셀(격자 칸) 수
struct FBuildingData
{
	GENERATED_BODY()

	// 건물이 차지하는 가로의 셀 수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Width;

	// 건물이 차지하는 세로의 셀 수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Height;

	// 건물에 대한 Enum 타입
	UPROPERTY()
	ECraftType BuildingType;

	FBuildingData()
		:Width(1), Height(1), BuildingType(ECraftType::None)
	{

	}
};