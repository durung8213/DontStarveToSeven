#pragma once

#include "CoreMinimal.h"
#include "Seojy/EnumCraftType.h"
#include "BuildingData.generated.h"

USTRUCT(BlueprintType)

// �� �ǹ��� �����ϰ� �� ����, ���� ��(���� ĭ) ��
struct FBuildingData
{
	GENERATED_BODY()

	// �ǹ��� �����ϴ� ������ �� ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Width;

	// �ǹ��� �����ϴ� ������ �� ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Height;

	// �ǹ��� ���� Enum Ÿ��
	UPROPERTY()
	ECraftType BuildingType;

	FBuildingData()
		:Width(1), Height(1), BuildingType(ECraftType::None)
	{

	}
};