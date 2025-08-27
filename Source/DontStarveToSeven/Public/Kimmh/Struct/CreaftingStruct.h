// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kimmh/Enum/CreaftingTypeEnum.h"
#include "Kimmh/Struct/ResourceItemStruct.h"
#include "Seojy/EnumCraftType.h"
#include "CreaftingStruct.generated.h"

/**
 * 
 */

USTRUCT(Atomic, BlueprintType)
struct DONTSTARVETOSEVEN_API FCreafting : public FTableRowBase {
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECreaftingType CreaftingType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* CreaftingImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FResourceItem> ResourceItemArr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Explan;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CreaftingTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECraftType CraftType;
};

//USTRUCT(Atomic, BlueprintType)
//struct DONTSTARVETOSEVEN_API FResourceItem : public FTableRowBase {
//	GENERATED_USTRUCT_BODY()
//
//public:
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	UTexture2D* ResourceItemImage;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	int ResoucreItemCount;
//};

class DONTSTARVETOSEVEN_API CreaftingStruct
{
public:
	CreaftingStruct();
	~CreaftingStruct();
};
