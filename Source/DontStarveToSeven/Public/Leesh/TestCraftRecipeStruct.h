// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TestCraftRecipeStruct.generated.h"


//// CraftingType 열거형 선언
//UENUM(BlueprintType)
//enum class ECraftingType : uint8
//{
//	None      UMETA(DisplayName = "None"),
//	Buildings UMETA(DisplayName = "Buildings"),
//	Items UMETA(DisplayName = "CraftItem") // 크래프트로 제작하는 아이템. (사실상 모든 아이템)
//};
//
//// BuildingType 열거형 선언
//UENUM(BlueprintType)
//enum class EBuildingType : uint8
//{
//	None      UMETA(DisplayName = "None"),
//	Turret UMETA(DisplayName = "Turret"),
//	Barricade UMETA(DisplayName = "Barricade"),
//	Shelter UMETA(DisplayName = "Shelter")
//};
//
//
//// CraftMaterial 구조체 선언
//USTRUCT(Atomic, BlueprintType)
//struct FTestCraftMaterial
//{
//	GENERATED_BODY()
//
//public:
//	UPROPERTY(BlueprintReadWrite, EditAnywhere)
//	FText ItemName;
//	UPROPERTY(BlueprintReadWrite, EditAnywhere)
//	int32 ItemCount;
//};
//
//
//
//// CraftRecipe 구조체 선언 (최상위 구조체)
//USTRUCT(Atomic, BlueprintType)
//struct FTestCraftRecipe : public FTableRowBase
//{
//	GENERATED_BODY()
//
//public:
//	UPROPERTY(BlueprintReadWrite, EditAnywhere)
//	ECraftingType CraftingType;
//	UPROPERTY(BlueprintReadWrite, EditAnywhere)
//	EBuildingType BuildingType;
//	UPROPERTY(BlueprintReadWrite, EditAnywhere)
//	TArray<FTestCraftMaterial> ResourceArr;
//	UPROPERTY(BlueprintReadWrite, EditAnywhere)
//	FText CraftInfo;
//	UPROPERTY(BlueprintReadWrite, EditAnywhere)
//	float CraftingDelay;
//};


UCLASS()
class DONTSTARVETOSEVEN_API UTestCraftRecipeStruct : public UObject
{
	GENERATED_BODY()
	
};
