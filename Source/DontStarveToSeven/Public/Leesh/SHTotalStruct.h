// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Leesh/SHBaseItem.h"
#include "Seojy/BaseCraft.h"
#include "SHTotalStruct.generated.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Inventory

// �κ��丮 Ÿ�� ������
UENUM(BlueprintType)
enum class EInventoryType : uint8
{
	None		  UMETA(DisplayName = "None"),
	Inventory     UMETA(DisplayName = "Inventory"),
	QuickSlot     UMETA(DisplayName = "QuickSlot")
};


// �κ��丮 ������ Ÿ�� 1�� (�󽽷� ����, ��ǥ��)
USTRUCT(Atomic, BlueprintType)
struct FCoordinate
{
	GENERATED_BODY()

public:

	// �κ��丮 �� � ��ġ�� �̹����� �־���ϴ��� ��ǥ�� �˷��ִ� ����ü

	// �������� �����ϰ� �ִ��� ����
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool FillSlot;
	// ������ X ��ǥ
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int X;
	// ������ Y ��ǥ
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Y;

	// �⺻ ������
	FCoordinate() : FillSlot(false), X(0), Y(0) {}
	// �Ű� ������ �ִ� ������ (�����ε�)
	FCoordinate(bool NewFillSlot, int NewX, int NewY) : FillSlot(NewFillSlot), X(NewX), Y(NewY) {}


	// ������ �����ε�
	FORCEINLINE bool operator==(const FCoordinate& Other) const
	{
		return X == Other.X && Y == Other.Y;
	}

};

// ���� �κ��丮�� �Ǵ� �迭�� �� �ε����� ����.
USTRUCT(Atomic, BlueprintType)
struct FInventory
{
	GENERATED_BODY()

	// ��ǥ�� �ʿ��� ��츸 ��� (��ǥ���� �����ϴ� ��Ȳ�̴� ��� ǥ��.)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseCoord = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FCoordinate InventoryCoordinate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString ItemName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int ItemCount;

	// �⺻ ������
	FInventory() : bUseCoord(false), InventoryCoordinate(), ItemName(TEXT("")), ItemCount(0) {}
	// �Ű� ������ �ִ� ������ (�����ε�)
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

	// ��� ���Կ� ��ģ ������ �� ����
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 TotalCount;

	// ���� � ���Ե鿡 �л�Ǿ� �ִ���
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FInventory> Slots;

	// �⺻ ������
	FSearchItemResult() : ItemName(TEXT("")), TotalCount(0), Slots() {}
	// �Ű� ������ �ִ� ������ (�����ε�)
	FSearchItemResult(FString NewItemName, int32 NewTotalCount, TArray<FInventory> NewSlots)
		: ItemName(NewItemName), TotalCount(NewTotalCount), Slots(NewSlots) {}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ItemData


// ItemType ������ ����
UENUM(BlueprintType)
enum class ETestItemType : uint8
{
	None          UMETA(DisplayName = "None"),
	Weapon        UMETA(DisplayName = "Weapon"), // ex) ��, Ȱ, ��, ����, ���, ��, ��ü�� ��
	Armor         UMETA(DisplayName = "Armor"),
	CraftMaterial UMETA(DisplayName = "CraftMaterial"),
	Consume       UMETA(DisplayName = "Consume"), // ex) ���� - ü��, ����, ���¹̳� / ���� ���� - ���ݼӵ�, �̵� �ӵ�
	Etc           UMETA(DisplayName = "Etc") // ex) ����, ����

};


// ItemStatChanges ����ü ���� (���� ����ü)
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


// �κ��丮 Ÿ�� ������
UENUM(BlueprintType)
enum class EItemGrade : uint8
{	
	Common		  UMETA(DisplayName = "Common"),
	Uncommon     UMETA(DisplayName = "Uncommon"),
	Rare     UMETA(DisplayName = "Rare"),
	Epic     UMETA(DisplayName = "Epic"),
	Legendary     UMETA(DisplayName = "Legendary"),
};

// ItemInfo ����ü (�ֻ��� ����ü)
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
	TSubclassOf<ASHBaseItem> Item; // BP ������ ��ü ��ü�� �����ϴ� ����.

};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Craft

// CraftingType ������ ����
UENUM(BlueprintType)
enum class ECraftingType : uint8
{
	None      UMETA(DisplayName = "None"),
	Buildings UMETA(DisplayName = "Buildings"),
	Items UMETA(DisplayName = "CraftItem") // ũ����Ʈ�� �����ϴ� ������. (��ǻ� ��� ������)
};

// BuildingType ������ ����
UENUM(BlueprintType)
enum class EBuildingType : uint8
{
	None      UMETA(DisplayName = "None"),
	Turret UMETA(DisplayName = "Turret"),
	Barricade UMETA(DisplayName = "Barricade"),
	Shelter UMETA(DisplayName = "Shelter")
};


// CraftMaterial ����ü ����
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



// CraftRecipe ����ü ���� (�ֻ��� ����ü)
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

// FBuildingInfoStruct ����ü ����
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
	// �⺻ ������, �Ű� ������ �ִ� ������ (�����ε�) ���� �ʿ�
};


// FNatureInfoStruct ����ü ����
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
	// �⺻ ������, �Ű� ������ �ִ� ������ (�����ε�) ���� �ʿ�
};



//// WorldInfo ����ü ����
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
