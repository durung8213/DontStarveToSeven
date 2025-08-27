// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TestItemStruct.generated.h"


//// ItemType ������ ����
//UENUM(BlueprintType)
//enum class ETestItemType : uint8
//{
//	None          UMETA(DisplayName = "None"),
//	Weapon        UMETA(DisplayName = "Weapon"), // ex) ��, Ȱ, ��, ����, ���, ��, ��ü�� ��
//	Armor         UMETA(DisplayName = "Armor"), 
//	CraftMaterial UMETA(DisplayName = "CraftMaterial"),
//	Consume       UMETA(DisplayName = "Consume"), // ex) ���� - ü��, ����, ���¹̳� / ���� ���� - ���ݼӵ�, �̵� �ӵ�
//	Etc           UMETA(DisplayName = "Etc") // ex) ����, ����
//
//};
//
//// ItemStatChanges ����ü ���� (���� ����ü)
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
//// ItemInfo ����ü (�ֻ��� ����ü)
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
//	TSoftObjectPtr<AActor> ItemObject; // BP ������ ��ü ��ü�� �����ϴ� ����.
//};




UCLASS()
class DONTSTARVETOSEVEN_API UTestItemStruct : public UObject
{
	GENERATED_BODY()
	
};
