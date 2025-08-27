// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TestItemStruct.generated.h"


//// ItemType 열거형 선언
//UENUM(BlueprintType)
//enum class ETestItemType : uint8
//{
//	None          UMETA(DisplayName = "None"),
//	Weapon        UMETA(DisplayName = "Weapon"), // ex) 검, 활, 총, 도끼, 곡괭이, 낫, 마체테 등
//	Armor         UMETA(DisplayName = "Armor"), 
//	CraftMaterial UMETA(DisplayName = "CraftMaterial"),
//	Consume       UMETA(DisplayName = "Consume"), // ex) 음식 - 체력, 마나, 스태미나 / 버프 포션 - 공격속도, 이동 속도
//	Etc           UMETA(DisplayName = "Etc") // ex) 열쇠, 코인
//
//};
//
//// ItemStatChanges 구조체 선언 (스탯 구조체)
//USTRUCT(Atomic, BlueprintType)
//struct FTestItemStatChanges
//{
//	GENERATED_BODY()
//
//public:
//	UPROPERTY(BlueprintReadWrite, EditAnywhere)
//	float MaxHP;
//	UPROPERTY(BlueprintReadWrite, EditAnywhere)
//	float MaxMP;
//	UPROPERTY(BlueprintReadWrite, EditAnywhere)
//	float Atk;
//	UPROPERTY(BlueprintReadWrite, EditAnywhere)
//	float Def;
//	UPROPERTY(BlueprintReadWrite, EditAnywhere)
//	float Speed;
//	UPROPERTY(BlueprintReadWrite, EditAnywhere)
//	float Critical;
//	UPROPERTY(BlueprintReadWrite, EditAnywhere)
//	float CriticalDamage;
//	UPROPERTY(BlueprintReadWrite, EditAnywhere)
//	float DurationTime;
//	UPROPERTY(BlueprintReadWrite, EditAnywhere)
//	int32 Tick;
//};
//
//
//// ItemInfo 구조체 (최상위 구조체)
//USTRUCT(Atomic, BlueprintType)
//struct FTestItemInfo : public FTableRowBase
//{
//	GENERATED_BODY()
//
//public:
//	UPROPERTY(BlueprintReadWrite, EditAnywhere)
//	FText ItemName;
//	UPROPERTY(BlueprintReadWrite, EditAnywhere)
//	FText ItemDescription;
//	UPROPERTY(BlueprintReadWrite, EditAnywhere)
//	ETestItemType ItemType;
//	UPROPERTY(BlueprintReadWrite, EditAnywhere)
//	TSoftObjectPtr<UTexture2D> ItemIcon;
//	UPROPERTY(BlueprintReadWrite, EditAnywhere)
//	int32 ItemMaxCount;
//	UPROPERTY(BlueprintReadWrite, EditAnywhere)
//	FTestItemStatChanges ItemStatChanges;
//	UPROPERTY(BlueprintReadWrite, EditAnywhere)
//	TSoftObjectPtr<AActor> ItemObject; // BP 아이템 객체 자체를 저장하는 변수.
//};




UCLASS()
class DONTSTARVETOSEVEN_API UTestItemStruct : public UObject
{
	GENERATED_BODY()
	
};
