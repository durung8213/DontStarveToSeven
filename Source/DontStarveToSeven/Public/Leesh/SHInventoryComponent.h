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
	// ������ �߰� �Լ�
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddItem(const FInventory& InItem, EInventoryType InventoryType = EInventoryType::None); // �κ��丮 Ÿ���� �������� ������, �⺻ �κ��丮�� ������ �߰�

	// �κ��丮 Ÿ�� Ȯ���ϴ� �Լ�
	TArray<FInventory>* GetInventoryArrayByType(EInventoryType Type);

	// �󽽷� �˻��ϴ� �Լ�
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int FindEmptySlot(TArray<FInventory>& InArray);

	// ������ ���� �Լ�
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveItem(const FInventory& InItem, EInventoryType InventoryType = EInventoryType::None);

	// ������ �ű�� �Լ�
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SwitchItem(const FInventory& OriginItem, EInventoryType OriginInventoryType = EInventoryType::None,
					const FInventory& TargetItem = FInventory(), EInventoryType TargetInventoryType = EInventoryType::None);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 SearchItemCount(FString InItemName, EInventoryType InventoryType = EInventoryType::None);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FSearchItemResult SearchItem(FString InItemName, EInventoryType InventoryType = EInventoryType::None);


	///	������ ���ø�����Ʈ �ϱ� ���� �ʿ��� �Լ�
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	// �κ��丮 �迭 (�����۸�� , ������ ����ִ��� �˻��Ͽ� ����ִٰ� �˷��� ��ǥ�� �� ����ü�� ��� �迭�� ���� ��)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Inventory, Category = "Inventory")
	TArray<FInventory> Inventory; // FInventory����ü�� �̿��� �迭 ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_QuickSlot, Category = "Inventory")
	TArray<FInventory> QuickSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Data")
	UDataTable* ItemDataTable;



	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void OnRep_Inventory();
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void OnRep_QuickSlot();


public:
	// �κ��丮 ũ�� ��� ����
	static constexpr int32 InventoryRows = 4; // Y = �� = ���� = Rows
	static constexpr int32 InventoryCols = 4; // X = �� = ���� = Cols
	static constexpr int32 InventorySize = InventoryRows * InventoryCols;

	// ������ ũ�� ��� ����
	static constexpr int32 QuickSlotRows = 1; // Y = �� = ���� = Rows
	static constexpr int32 QuickSlotCols = 6; // X = �� = ���� = Cols
	static constexpr int32 QuickSlotSize = QuickSlotRows * QuickSlotCols;


};
