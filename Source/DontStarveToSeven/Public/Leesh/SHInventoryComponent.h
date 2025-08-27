// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SHTotalStruct.h"
#include "SHInventoryComponent.generated.h"



UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DONTSTARVETOSEVEN_API USHInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USHInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;



public:
	// 아이템 추가 함수
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddItem(const FInventory& InItem, EInventoryType InventoryType = EInventoryType::None); // 인벤토리 타입을 지정하지 않으면, 기본 인벤토리로 아이템 추가

	// 인벤토리 타입 확인하는 함수
	TArray<FInventory>* GetInventoryArrayByType(EInventoryType Type);

	// 빈슬롯 검사하는 함수
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int FindEmptySlot(TArray<FInventory>& InArray);

	// 아이템 제거 함수
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveItem(const FInventory& InItem, EInventoryType InventoryType = EInventoryType::None);

	// 아이템 옮기는 함수
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SwitchItem(const FInventory& OriginItem, EInventoryType OriginInventoryType = EInventoryType::None,
					const FInventory& TargetItem = FInventory(), EInventoryType TargetInventoryType = EInventoryType::None);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 SearchItemCount(FString InItemName, EInventoryType InventoryType = EInventoryType::None);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FSearchItemResult SearchItem(FString InItemName, EInventoryType InventoryType = EInventoryType::None);


	///	변수를 리플리케이트 하기 위해 필요한 함수
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	// 인벤토리 배열 (아이템명과 , 위에서 비어있는지 검사하여 비어있다고 알려진 좌표값 을 구조체로 묶어서 배열로 만든 것)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Inventory, Category = "Inventory")
	TArray<FInventory> Inventory; // FInventory구조체를 이용한 배열 선언

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_QuickSlot, Category = "Inventory")
	TArray<FInventory> QuickSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Data")
	UDataTable* ItemDataTable;



	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void OnRep_Inventory();
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void OnRep_QuickSlot();


public:
	// 인벤토리 크기 상수 정의
	static constexpr int32 InventoryRows = 4; // Y = 행 = 가로 = Rows
	static constexpr int32 InventoryCols = 4; // X = 열 = 세로 = Cols
	static constexpr int32 InventorySize = InventoryRows * InventoryCols;

	// 퀵슬롯 크기 상수 정의
	static constexpr int32 QuickSlotRows = 1; // Y = 행 = 가로 = Rows
	static constexpr int32 QuickSlotCols = 6; // X = 열 = 세로 = Cols
	static constexpr int32 QuickSlotSize = QuickSlotRows * QuickSlotCols;


};
