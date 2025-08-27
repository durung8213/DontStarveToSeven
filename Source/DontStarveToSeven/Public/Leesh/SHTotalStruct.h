// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Leesh/SHBaseItem.h"
#include "Seojy/BaseCraft.h"
#include "SHTotalStruct.generated.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Inventory

// 인벤토리 타입 열거형
UENUM(BlueprintType)
enum class EInventoryType : uint8
{
	None		  UMETA(DisplayName = "None"),
	Inventory     UMETA(DisplayName = "Inventory"),
	QuickSlot     UMETA(DisplayName = "QuickSlot")
};


// 인벤토리 슬롯의 타일 1개 (빈슬롯 여부, 좌표값)
USTRUCT(Atomic, BlueprintType)
struct FCoordinate
{
	GENERATED_BODY()

public:

	// 인벤토리 상에 어떤 위치에 이미지를 넣어야하는지 좌표를 알려주는 구조체

	// 아이템이 차지하고 있는지 여부
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool FillSlot;
	// 슬롯의 X 좌표
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int X;
	// 슬롯의 Y 좌표
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Y;

	// 기본 생성자
	FCoordinate() : FillSlot(false), X(0), Y(0) {}
	// 매개 변수가 있는 생성자 (오버로딩)
	FCoordinate(bool NewFillSlot, int NewX, int NewY) : FillSlot(NewFillSlot), X(NewX), Y(NewY) {}


	// 연산자 오버로딩
	FORCEINLINE bool operator==(const FCoordinate& Other) const
	{
		return X == Other.X && Y == Other.Y;
	}

};

// 실제 인벤토리가 되는 배열의 각 인덱스의 내용.
USTRUCT(Atomic, BlueprintType)
struct FInventory
{
	GENERATED_BODY()

	// 좌표가 필요한 경우만 사용 (좌표값을 전달하는 상황이다 라는 표시.)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseCoord = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FCoordinate InventoryCoordinate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString ItemName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int ItemCount;

	// 기본 생성자
	FInventory() : bUseCoord(false), InventoryCoordinate(), ItemName(TEXT("")), ItemCount(0) {}
	// 매개 변수가 있는 생성자 (오버로딩)
	FInventory(bool NewbUseCoord, FCoordinate NewCoordinate, FString NewItemName, int NewItemCount)
		: bUseCoord(NewbUseCoord), InventoryCoordinate(NewCoordinate), ItemName(NewItemName), ItemCount(NewItemCount) {}

};


USTRUCT(Atomic, BlueprintType)
struct FSearchItemResult
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString ItemName;

	// 모든 슬롯에 걸친 아이템 총 개수
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 TotalCount;

	// 실제 어떤 슬롯들에 분산되어 있는지
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FInventory> Slots;

	// 기본 생성자
	FSearchItemResult() : ItemName(TEXT("")), TotalCount(0), Slots() {}
	// 매개 변수가 있는 생성자 (오버로딩)
	FSearchItemResult(FString NewItemName, int32 NewTotalCount, TArray<FInventory> NewSlots)
		: ItemName(NewItemName), TotalCount(NewTotalCount), Slots(NewSlots) {}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ItemData


// ItemType 열거형 선언
UENUM(BlueprintType)
enum class ETestItemType : uint8
{
	None          UMETA(DisplayName = "None"),
	Weapon        UMETA(DisplayName = "Weapon"), // ex) 검, 활, 총, 도끼, 곡괭이, 낫, 마체테 등
	Armor         UMETA(DisplayName = "Armor"),
	CraftMaterial UMETA(DisplayName = "CraftMaterial"),
	Consume       UMETA(DisplayName = "Consume"), // ex) 음식 - 체력, 마나, 스태미나 / 버프 포션 - 공격속도, 이동 속도
	Etc           UMETA(DisplayName = "Etc") // ex) 열쇠, 코인

};


// ItemStatChanges 구조체 선언 (스탯 구조체)
USTRUCT(Atomic, BlueprintType)
struct FTestItemStatChanges
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxHP;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxMP;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Atk;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Def;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Speed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Critical;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CriticalDamage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float DurationTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Tick;

	FTestItemStatChanges() : MaxHP(0.0), MaxMP(0.0), Atk(0.0), Def(0.0), Speed(0.0), Critical(0.0), CriticalDamage(0.0), DurationTime(0.0), Tick(0) {}
	FTestItemStatChanges(float NewMaxHP, float NewMaxMP, float NewAtk, float NewDef, float NewSpeed, float NewCritical, float NewCriticalDamage, float NewDurationTime, int32 NewTick)
		: MaxHP(NewMaxHP), MaxMP(NewMaxMP), Atk(NewAtk), Def(NewDef), Speed(NewSpeed), Critical(NewCritical), CriticalDamage(NewCriticalDamage), DurationTime(NewDurationTime), Tick(NewTick) {}

};


// 인벤토리 타입 열거형
UENUM(BlueprintType)
enum class EItemGrade : uint8
{	
	Common		  UMETA(DisplayName = "Common"),
	Uncommon     UMETA(DisplayName = "Uncommon"),
	Rare     UMETA(DisplayName = "Rare"),
	Epic     UMETA(DisplayName = "Epic"),
	Legendary     UMETA(DisplayName = "Legendary"),
};

// ItemInfo 구조체 (최상위 구조체)
USTRUCT(Atomic, BlueprintType)
struct FTestItemInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText ItemName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText ItemDescription;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ETestItemType ItemType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftObjectPtr<UTexture2D> ItemIcon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 ItemMaxCount;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EItemGrade ItemGrade;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText ItemDropPath;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTestItemStatChanges ItemStatChanges;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<ASHBaseItem> Item; // BP 아이템 객체 자체를 저장하는 변수.

};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Craft

// CraftingType 열거형 선언
UENUM(BlueprintType)
enum class ECraftingType : uint8
{
	None      UMETA(DisplayName = "None"),
	Buildings UMETA(DisplayName = "Buildings"),
	Items UMETA(DisplayName = "CraftItem") // 크래프트로 제작하는 아이템. (사실상 모든 아이템)
};

// BuildingType 열거형 선언
UENUM(BlueprintType)
enum class EBuildingType : uint8
{
	None      UMETA(DisplayName = "None"),
	Turret UMETA(DisplayName = "Turret"),
	Barricade UMETA(DisplayName = "Barricade"),
	Shelter UMETA(DisplayName = "Shelter")
};


// CraftMaterial 구조체 선언
USTRUCT(Atomic, BlueprintType)
struct FCraftMaterial
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText ItemName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 ItemCount;
};



// CraftRecipe 구조체 선언 (최상위 구조체)
USTRUCT(Atomic, BlueprintType)
struct FCraftRecipe : public FTableRowBase
{
	GENERATED_BODY()

public:
	//UPROPERTY(BlueprintReadWrite, EditAnywhere)
	//ECraftingType CraftingType;
	//UPROPERTY(BlueprintReadWrite, EditAnywhere)
	//EBuildingType BuildingType;
	//UPROPERTY(BlueprintReadWrite, EditAnywhere)
	//TArray<FCraftMaterial> ResourceArr;
	//UPROPERTY(BlueprintReadWrite, EditAnywhere)
	//FText CraftInfo;
	//UPROPERTY(BlueprintReadWrite, EditAnywhere)
	//float CraftingDelay;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString CraftName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<ABaseCraft> CraftClass;

};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PlayerInfo


USTRUCT(Atomic, BlueprintType)
struct FCharacterStat
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxHP;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CurrentHP;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxMp;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CurrentMp;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Atk;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Def;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Speed;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Critical;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CriticalDamage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxHunger;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CurrentHunger;


	FCharacterStat() : MaxHP(0.0), CurrentHP(0.0), MaxMp(0.0), CurrentMp(0.0), Atk(0.0), Def(0.0), Speed(0.0), Critical(0.0), CriticalDamage(0.0), MaxHunger(0.0), CurrentHunger(0.0) {}
	FCharacterStat(float NewMaxHP, float NewCurrentHP, float NewMaxMp, float NewCurrentMp, float NewAtk, float NewDef, float NewSpeed, float NewCritical, float NewCriticalDamage, float NewMaxHunger, float NewCurrentHunger)
		: MaxHP(NewMaxHP), CurrentHP(NewCurrentHP), MaxMp(NewMaxMp), CurrentMp(NewCurrentMp), Atk(NewAtk), Def(NewDef), Speed(NewSpeed), Critical(NewCritical), CriticalDamage(NewCriticalDamage), MaxHunger(NewMaxHunger), CurrentHunger(NewCurrentHunger) {}
};

USTRUCT(Atomic, BlueprintType)
struct FPlayerInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FCharacterStat Stat;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FInventory> Inven;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FInventory> Quick;

	FPlayerInfo() : Stat(), Inven(), Quick() {}
	FPlayerInfo(FCharacterStat NewStat, TArray<FInventory> NewInven, TArray<FInventory> NewQuick)
		: Stat(NewStat), Inven(NewInven), Quick(NewQuick) {}

};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//WorldInfo

// FBuildingInfoStruct 구조체 선언
USTRUCT(Atomic, BlueprintType)
struct FBuildingInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString BuildingClassName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float BuildingHP;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTransform BuildingTransform;
	// 기본 생성자, 매개 변수가 있는 생성자 (오버로딩) 정의 필요
};


// FNatureInfoStruct 구조체 선언
USTRUCT(Atomic, BlueprintType)
struct FNatureInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString NatureClassName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float NatureHP;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTransform NatureTransform;
	// 기본 생성자, 매개 변수가 있는 생성자 (오버로딩) 정의 필요
};



//// WorldInfo 구조체 선언
//USTRUCT(Atomic, BlueprintType)
//struct FWorldInfo
//{
//	GENERATED_BODY()
//
//public:
//	UPROPERTY(BlueprintReadWrite, EditAnywhere)
//	TMap<FGuid, FBuildingInfo> BuildingInfoMapPack;
//	UPROPERTY(BlueprintReadWrite, EditAnywhere)
//	TMap<FGuid, FNatureInfo> NatureInfoMapPack;
//};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//GameMode Total Data

USTRUCT(Atomic, BlueprintType)
	struct FTotalData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<FString, FPlayerInfo> PlayerInfoMapPack;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<FGuid, FBuildingInfo> BuildingInfoMapPack;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<FGuid, FNatureInfo> NatureInfoMapPack;
};





UCLASS()
class DONTSTARVETOSEVEN_API USHTotalStruct : public UObject
{
	GENERATED_BODY()
	

	USHTotalStruct();
};
