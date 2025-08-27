// Fill out your copyright notice in the Description page of Project Settings.


#include "Leesh/SHInventoryComponent.h"
#include "Kimmh/Character/Controller/DTS_GamePlayPlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
USHInventoryComponent::USHInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	

	// �κ��丮 ũ�� ����.
	Inventory.SetNum(USHInventoryComponent::InventorySize);
	UE_LOG(LogTemp, Warning, TEXT("Inventory array size: %d"), Inventory.Num());


	// �κ��丮 �󽽷�,��ǥ �ʱ�ȭ
	if (Inventory.Num() >= USHInventoryComponent::InventorySize) // ������ üũ
	{
		for (int i = 0; i < USHInventoryComponent::InventoryRows; i++)
		{
			for (int j = 0; j < USHInventoryComponent::InventoryCols; j++)
			{
				Inventory[i * USHInventoryComponent::InventoryCols + j].InventoryCoordinate = FCoordinate(false, j, i);
				UE_LOG(LogTemp, Warning, TEXT("Inventory Coordinate Initializing: %d , %d"), 
				Inventory[i * USHInventoryComponent::InventoryCols + j].InventoryCoordinate.Y, 
				Inventory[i * USHInventoryComponent::InventoryCols + j].InventoryCoordinate.X);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Inventory array was not correctly initialized!"));
	}


	// ������ ũ�� ����.
	QuickSlot.SetNum(USHInventoryComponent::QuickSlotSize);
	UE_LOG(LogTemp, Warning, TEXT("QuickSlot array size: %d"), QuickSlot.Num());

	// ������ �󽽷�,��ǥ �ʱ�ȭ
	if (QuickSlot.Num() >= USHInventoryComponent::QuickSlotSize) // ������ üũ
	{
		for (int i = 0; i < USHInventoryComponent::QuickSlotRows; i++)
		{
			for (int j = 0; j < USHInventoryComponent::QuickSlotCols; j++)
			{
				QuickSlot[i * USHInventoryComponent::QuickSlotCols + j].InventoryCoordinate = FCoordinate(false, j, i);
				UE_LOG(LogTemp, Warning, TEXT("QuickSlot Coordinate Initializing: %d , %d"),
					QuickSlot[i * USHInventoryComponent::QuickSlotCols + j].InventoryCoordinate.Y,
					QuickSlot[i * USHInventoryComponent::QuickSlotCols + j].InventoryCoordinate.X);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("QuickSlot array was not correctly initialized!"));
	}


}

// Called when the game starts
void USHInventoryComponent::BeginPlay()
{
	Super::BeginPlay();



}

// Called every frame
void USHInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}



void USHInventoryComponent::AddItem(const FInventory& InItem, EInventoryType InventoryType)
{
	// InventoryType�� None�̸� ��ȿ���� ���� Ÿ������ ó��
	if (InventoryType == EInventoryType::None)
	{
		UE_LOG(LogTemp, Error, TEXT("[RemoveItem] Invalid InventoryType: None"));
		return;
	}
	
	// ������ ������ ���̺��� ���� ��. ������ ����.
	if (!ItemDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("ItemDataTable is null"));
		return;
	}

	// ���̺��� �Է� ���� ������ ã��.
	const FName RowName(*InItem.ItemName);
	const FTestItemInfo* ItemInfo = ItemDataTable->FindRow<FTestItemInfo>(RowName, TEXT(""));

	// ���̺� ���� �������� ��. ������ ����.
	if (!ItemInfo)
	{
		UE_LOG(LogTemp, Error, TEXT("Item not found in DataTable: %s"), *InItem.ItemName);
		return;
	}

	const int32 MaxCount = ItemInfo->ItemMaxCount; // ���̺��� MaxCount�� ������ ����.
	int32 RemainingCount = InItem.ItemCount; // �Է� ���� ������ ������ ������ ����.

	&QuickSlot;
	TArray<FInventory>* TargetArray = GetInventoryArrayByType(InventoryType); // �߰��Ǵ� �κ��丮 Ÿ���� ������ ����.
	if (!TargetArray)
	{
		UE_LOG(LogTemp, Error, TEXT("[AddItem] Invalid inventory type"));
		return;
	}

	// 1. ��ǥ ����� ���, ��Ȯ�� ��ġ�� �����
	if (InItem.bUseCoord)
	{
		// �Է� ���� ��ǥ�� �ε��� ã��
		int32 Index = TargetArray->IndexOfByPredicate([&](const FInventory& Slot)
			{
				return Slot.InventoryCoordinate == InItem.InventoryCoordinate;
			});

		if (TargetArray->IsValidIndex(Index))
		{
			FInventory& Slot = (*TargetArray)[Index];

			// 1-1. ������ ���� �������̸� Count �߰�
			if (Slot.ItemName == InItem.ItemName || Slot.ItemName.IsEmpty())
			{
				int32 Addable = FMath::Min(MaxCount - Slot.ItemCount, RemainingCount);
				Slot.ItemName = InItem.ItemName;
				Slot.ItemCount += Addable;
				Slot.InventoryCoordinate.FillSlot = true;
				Slot.bUseCoord = true;
				RemainingCount -= Addable;

				UE_LOG(LogTemp, Warning, TEXT("[AddItem Coord] +%d to slot (%d,%d)"), Addable, Slot.InventoryCoordinate.X, Slot.InventoryCoordinate.Y);
				UE_LOG(LogTemp, Warning, TEXT("Total : [%s] %d (%d,%d)"), *Slot.ItemName, Slot.ItemCount, Slot.InventoryCoordinate.X, Slot.InventoryCoordinate.Y);
			}
			else
			{
				// 1-2. �ٸ� �������� �̹� �ִ� ��� -> ����� �Ұ�
				UE_LOG(LogTemp, Error, TEXT("[AddItem Coord] Cannot overwrite slot (%d,%d) with different item (%s vs %s)"),
					Slot.InventoryCoordinate.X, Slot.InventoryCoordinate.Y,
					*Slot.ItemName, *InItem.ItemName);
				return;
			}

		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[AddItem Coord] Invalid target coord (%d,%d)"), InItem.InventoryCoordinate.X, InItem.InventoryCoordinate.Y);
			return;
		}
	}
	// 2. ��ǥ ������ �ڵ� ��ġ
	if (!InItem.bUseCoord && RemainingCount > 0)
	{
		// 2-1. ���� ���� ���Կ� ���� ������ �ִ� ��� -> ���� �߰�
		for (FInventory& Slot : *TargetArray)
		{
			if (Slot.ItemName == InItem.ItemName && Slot.ItemCount < MaxCount)
			{
				int32 Addable = FMath::Min(MaxCount - Slot.ItemCount, RemainingCount);
				Slot.ItemCount += Addable;
				Slot.InventoryCoordinate.FillSlot = true;
				RemainingCount -= Addable;

				UE_LOG(LogTemp, Warning, TEXT("[AddItem Stack] +%d to existing (%d,%d)"), Addable, Slot.InventoryCoordinate.X, Slot.InventoryCoordinate.Y);
				UE_LOG(LogTemp, Warning, TEXT("Total : [%s] %d (%d,%d)"), *Slot.ItemName, Slot.ItemCount, Slot.InventoryCoordinate.X, Slot.InventoryCoordinate.Y);

			}

			if (RemainingCount <= 0)
			{
				break;
			}
		}
		// 2-2. ���� ������ �� ���Կ� ���� ä���
		while (RemainingCount > 0)
		{
			int32 EmptyIndex = FindEmptySlot(*TargetArray);
			if (!TargetArray->IsValidIndex(EmptyIndex))
			{
				UE_LOG(LogTemp, Error, TEXT("[AddItem Auto] No empty slot for %s, %d remaining"), *InItem.ItemName, RemainingCount);
				break;
			}

			FInventory& NewSlot = (*TargetArray)[EmptyIndex];
			NewSlot.ItemName = InItem.ItemName;
			NewSlot.ItemCount = FMath::Min(RemainingCount, MaxCount);
			NewSlot.InventoryCoordinate.FillSlot = true;
			NewSlot.bUseCoord = false;
			RemainingCount -= NewSlot.ItemCount;

			UE_LOG(LogTemp, Warning, TEXT("[AddItem Auto] Added %s (%d) to (%d,%d)"), *NewSlot.ItemName, NewSlot.ItemCount, NewSlot.InventoryCoordinate.X, NewSlot.InventoryCoordinate.Y);
			UE_LOG(LogTemp, Warning, TEXT("Total : [%s] %d (%d,%d)"), *NewSlot.ItemName, NewSlot.ItemCount, NewSlot.InventoryCoordinate.X, NewSlot.InventoryCoordinate.Y);

		}
	}
	FString Message = FString::Printf(TEXT("ItemAdd Success"));
	UKismetSystemLibrary::PrintString(this, Message);
}

TArray<FInventory>* USHInventoryComponent::GetInventoryArrayByType(EInventoryType Type)
{
	switch (Type)
	{
	case EInventoryType::Inventory:
		UE_LOG(LogTemp, Log, TEXT("Inventory Type."));
		return &Inventory;

	case EInventoryType::QuickSlot:
		UE_LOG(LogTemp, Log, TEXT("QuickSlot Type."));
		return &QuickSlot;

	default:
		UE_LOG(LogTemp, Log, TEXT("Invalid Inventory Type."));
		return nullptr;
	}
}


int USHInventoryComponent::FindEmptySlot(TArray<FInventory>& InArray)
{

	// Inventory �迭�� ��ȸ�ϸ鼭, �󽽷� �˻��Ͽ� ��ȯ.
	for (int32 Index = 0; Index < InArray.Num(); ++Index)
	{
		if (!InArray[Index].InventoryCoordinate.FillSlot)
		{
			UE_LOG(LogTemp, Warning, TEXT("FindIndexNum: %s"), *FString::FromInt(Index));
			UE_LOG(LogTemp, Warning, TEXT("TempSlot item: (%d , %d)"), InArray[Index].InventoryCoordinate.X, InArray[Index].InventoryCoordinate.Y);
			return Index;
		}

	}
	// �� ������ ���� ��� -1 ��ȯ
	return -1;
}

void USHInventoryComponent::RemoveItem(const FInventory& InItem, EInventoryType InventoryType)
{
	// InventoryType�� None�̸� ��ȿ���� ���� Ÿ������ ó��
	if (InventoryType == EInventoryType::None)
	{
		UE_LOG(LogTemp, Error, TEXT("[RemoveItem] Invalid InventoryType: None"));
		return;
	}	
	
	TArray<FInventory>* TargetArray = GetInventoryArrayByType(InventoryType);

	if (!TargetArray || InItem.ItemName.IsEmpty() || InItem.ItemCount <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("[RemoveItem] Invalid parameters."));
		return;
	}

	int32 RemainingToRemove = InItem.ItemCount;
	

	// 1. ��ǥ ��� ����
	if (InItem.bUseCoord)
	{
		// �Էµ� ��ǥ �������� �ε��� ã��.
		int32 TargetIndex = TargetArray->IndexOfByPredicate([&](const FInventory& Slot)
		{
				return Slot.InventoryCoordinate == InItem.InventoryCoordinate;
		});

		// �����Ϸ��� ��ǥ�� ���� ��ǥ�� ��(�߸��� ��ǥ �Է�)
		if (!TargetArray->IsValidIndex(TargetIndex))
		{
			UE_LOG(LogTemp, Error, TEXT("[RemoveItem Coord] Invalid coord (%d,%d)"),
				InItem.InventoryCoordinate.X, InItem.InventoryCoordinate.Y);
			return;
		}
		// �����Ϸ��� �����۸��� ���� ��.
		FInventory& Slot = (*TargetArray)[TargetIndex];
		if (Slot.ItemName != InItem.ItemName)
		{
			UE_LOG(LogTemp, Error, TEXT("[RemoveItem Coord] Item mismatch at (%d,%d): %s vs %s"),
				Slot.InventoryCoordinate.X, Slot.InventoryCoordinate.Y,
				*Slot.ItemName, *InItem.ItemName);
			return;
		}
		// ������ ������ ������ ���ڶ� ��.
		if (Slot.ItemCount < RemainingToRemove)
		{
			UE_LOG(LogTemp, Warning, TEXT("[RemoveItem Coord] Not enough items to remove."));
			return;
		}

		// ���� ����.
		Slot.ItemCount -= RemainingToRemove;

		// ���� �� ó��.
		if (Slot.ItemCount == 0)
		{
			Slot = FInventory(true, Slot.InventoryCoordinate, TEXT(""), 0);
			Slot.InventoryCoordinate.FillSlot = false;
		}

		UE_LOG(LogTemp, Warning, TEXT("[RemoveItem Coord] Removed %d from (%d,%d)"),
			RemainingToRemove, Slot.InventoryCoordinate.X, Slot.InventoryCoordinate.Y);
		return;
	}

	// 2. �����۸� ��� ��ü ���� (���� ���� �л� ���� ����)
	for (FInventory& Slot : *TargetArray)
	{
		// ������ ������ ���� �ʾҴٸ� ���� Ż��.
		if (RemainingToRemove <= 0)
		{
			break;
		}
		// ��ü �κ��丮���� �����Ϸ��� �����۸����� ã�Ƽ� ����.
		if (Slot.ItemName == InItem.ItemName)
		{
			if (Slot.ItemCount > RemainingToRemove)
			{
				Slot.ItemCount -= RemainingToRemove;
				UE_LOG(LogTemp, Warning, TEXT("[RemoveItem Name] Removed %d from (%d,%d)"),
					RemainingToRemove, Slot.InventoryCoordinate.X, Slot.InventoryCoordinate.Y);
				break;
			}
			else
			{
				RemainingToRemove -= Slot.ItemCount;
				Slot = FInventory(false, Slot.InventoryCoordinate, TEXT(""), 0);
				Slot.InventoryCoordinate.FillSlot = false;

				UE_LOG(LogTemp, Warning, TEXT("[RemoveItem Name] Emptied slot (%d,%d)"),
					Slot.InventoryCoordinate.X, Slot.InventoryCoordinate.Y);
			}
		}
	}

	if (RemainingToRemove > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("[RemoveItem Name] Not all items removed. %d remaining."), RemainingToRemove);
	}

}

void USHInventoryComponent::SwitchItem(const FInventory& OriginItem, EInventoryType OriginInventoryType, 
									   const FInventory& TargetItem, EInventoryType TargetInventoryType)
{
	// �κ��丮 Ÿ���� ��ȿ���� Ȯ�� (None�̸� ���� ó��)
	if (OriginInventoryType == EInventoryType::None || TargetInventoryType == EInventoryType::None)
	{
		UE_LOG(LogTemp, Error, TEXT("[SwitchItem] Invalid inventory type provided. OriginType: %d, TargetType: %d"),
			(uint8)OriginInventoryType, (uint8)TargetInventoryType);
		return;
	}
	
	// 0. �κ��丮 �迭 ��������
	TArray<FInventory>* OriginArray = GetInventoryArrayByType(OriginInventoryType);
	TArray<FInventory>* TargetArray = GetInventoryArrayByType(TargetInventoryType);
	if (!OriginArray || !TargetArray)
	{
		UE_LOG(LogTemp, Error, TEXT("[SwitchItem] Invalid Origin/Target Array."));
		return;
	}

	// 1. OriginSlot, TargetSlot �ε��� ã��
	const int32 OriginIndex = OriginArray->IndexOfByPredicate([&](const FInventory& Slot)
	{
		return Slot.InventoryCoordinate == OriginItem.InventoryCoordinate;
	}
	);
	const int32 TargetIndex = TargetArray->IndexOfByPredicate([&](const FInventory& Slot)
	{
		return Slot.InventoryCoordinate == TargetItem.InventoryCoordinate;
	}
	);

	// ��ȿ�� �˻�
	if (!OriginArray->IsValidIndex(OriginIndex) || !TargetArray->IsValidIndex(TargetIndex))
	{
		UE_LOG(LogTemp, Error, TEXT("[SwitchItem] Invalid indices for Origin/Target."));
		return;
	}

	FInventory& OriginSlot = (*OriginArray)[OriginIndex];
	FInventory& TargetSlot = (*TargetArray)[TargetIndex];

	// ���� �ƹ��͵� ���� ������ �߸� Ŭ���߰ų�, Origin ��ü�� ��������� ����
	if (OriginSlot.ItemCount <= 0 || OriginSlot.ItemName.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("[SwitchItem] Origin is empty. Nothing to move."));
		return;
	}
	
	// ���� ���콺�� ���ڸ��� ���� ��� ����. (���� �ڸ����� ����ٰ� ���� ����.)
	if (OriginInventoryType == TargetInventoryType)
	{
		if (OriginIndex == TargetIndex)
		{
			UE_LOG(LogTemp, Warning, TEXT("[SwitchItem] Same slot selected. No switch performed."));
			return;
		}
	}


	// ������ ������ ���̺� ��ȸ (���� ���� �� �ʿ�)
	if (!ItemDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("[SwitchItem] ItemDataTable is null."));
		return;
	}

	// 2. ���� �������̸� ���� ����
	if (!TargetSlot.ItemName.IsEmpty() && (OriginSlot.ItemName == TargetSlot.ItemName))
	{
		// ������ ���̺��� ������ ���� �˻�
		const FTestItemInfo* ItemInfo = ItemDataTable->FindRow<FTestItemInfo>(
			FName(*OriginSlot.ItemName),
			TEXT("[SwitchItem]")
		);
		if (!ItemInfo)
		{
			UE_LOG(LogTemp, Warning, TEXT("[SwitchItem] Cannot find item info in DataTable."));
			return;
		}

		const int32 MaxCount = ItemInfo->ItemMaxCount;
		const int32 TotalCount = OriginSlot.ItemCount + TargetSlot.ItemCount;

		// ��ü ������ MaxCount ���϶�� Target�� ���Ƴְ� Origin�� ����
		if (TotalCount <= MaxCount)
		{
			TargetSlot.ItemCount = TotalCount;

			// Origin ���� �ʱ�ȭ
			OriginSlot.ItemName = TEXT("");
			OriginSlot.ItemCount = 0;
			OriginSlot.InventoryCoordinate.FillSlot = false;  // ���� ������� ǥ��
			// bUseCoord�� �巡�׾ص�ӿ����� ���� true�� ������ ���� ������, 
			// �ϴ� �������� ����� ������ FillSlot=false�� ó���ϸ� ���.

			UE_LOG(LogTemp, Warning, TEXT("[SwitchItem] Stack merged into Target (%d,%d). Total: %d"),
				TargetSlot.InventoryCoordinate.X, TargetSlot.InventoryCoordinate.Y, TargetSlot.ItemCount);
		}
		else
		{
			// TargetSlot�� MaxCount�� ä���, �������� Origin�� ����
			TargetSlot.ItemCount = MaxCount;
			OriginSlot.ItemCount = TotalCount - MaxCount;

			UE_LOG(LogTemp, Warning, TEXT("[SwitchItem] Partial merge. Target:%d, Origin:%d"),
				TargetSlot.ItemCount, OriginSlot.ItemCount);
		}
		return;
	}

	// 3. Target�� �� �����̸� �� �ܼ� �̵�
	if (TargetSlot.ItemName.IsEmpty())
	{
		// Target ���Կ� Origin ������ ���� ����
		TargetSlot = OriginSlot;
		TargetSlot.InventoryCoordinate = TargetItem.InventoryCoordinate;

		// Origin ���� �ʱ�ȭ
		OriginSlot.ItemName = TEXT("");
		OriginSlot.ItemCount = 0;
		OriginSlot.InventoryCoordinate.FillSlot = false;

		UE_LOG(LogTemp, Warning, TEXT("[SwitchItem] Moved item to empty Target slot (%d,%d)."),
			TargetSlot.InventoryCoordinate.X, TargetSlot.InventoryCoordinate.Y);

		return;
	}

	// 4. �ٸ� �������̸� �� ���� ��ü (Swap)
	{
		// ��ǥ�� ���� �ٲ� �� ��, ���� ���� ��ȯ
		FCoordinate TempCoord = OriginSlot.InventoryCoordinate;
		OriginSlot.InventoryCoordinate = TargetSlot.InventoryCoordinate;
		TargetSlot.InventoryCoordinate = TempCoord;

		// ���� ������ ���� ��ȯ
		Swap(OriginSlot, TargetSlot);

		UE_LOG(LogTemp, Warning, TEXT("[SwitchItem] Swapped different items: Origin(%d,%d) <-> Target(%d,%d)"),
			TargetSlot.InventoryCoordinate.X, TargetSlot.InventoryCoordinate.Y,
			OriginSlot.InventoryCoordinate.X, OriginSlot.InventoryCoordinate.Y);
	}
	


}

int32 USHInventoryComponent::SearchItemCount(FString InItemName, EInventoryType InventoryType)
{
	// InventoryType�� None�̸� ��ȿ���� ���� Ÿ������ ó��
	if (InventoryType == EInventoryType::None)
	{
		UE_LOG(LogTemp, Error, TEXT("[RemoveItem] Invalid InventoryType: None"));
		return 0;
	}

	if (InItemName.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("[SearchItemCount] Invalid ItemName."));
		return 0;
	}

	TArray<FInventory>* TargetArray = GetInventoryArrayByType(InventoryType);
	if (!TargetArray)
	{
		UE_LOG(LogTemp, Warning, TEXT("[SearchItemCount] Invalid Inventory Type."));
		return 0;
	}

	int32 TotalCount = 0;

	for (const FInventory& Slot : *TargetArray)
	{
		if (Slot.ItemName == InItemName)
		{
			TotalCount += Slot.ItemCount;
		}
	}

	return TotalCount;
}

FSearchItemResult USHInventoryComponent::SearchItem(FString InItemName, EInventoryType InventoryType)
{
	// InventoryType�� None�̸� ��ȿ���� ���� Ÿ������ ó��
	if (InventoryType == EInventoryType::None)
	{
		UE_LOG(LogTemp, Error, TEXT("[RemoveItem] Invalid InventoryType: None"));
		return FSearchItemResult();  // �⺻�� ��ȯ;
	}

	FSearchItemResult Result;
	Result.ItemName = InItemName;
	Result.TotalCount = 0;

	if (InItemName.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("[SearchItem] Invalid ItemName."));
		return Result;
	}

	TArray<FInventory>* TargetArray = GetInventoryArrayByType(InventoryType);
	if (!TargetArray)
	{
		UE_LOG(LogTemp, Warning, TEXT("[SearchItem] Invalid Inventory Type."));
		return Result;
	}

	for (const FInventory& Slot : *TargetArray)
	{
		if (Slot.ItemName == InItemName)
		{
			Result.TotalCount += Slot.ItemCount;
			Result.Slots.Add(Slot);
		}
	}

	return Result;

}





void USHInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USHInventoryComponent, Inventory); // ���� ��� �ʼ�
	DOREPLIFETIME(USHInventoryComponent, QuickSlot); // ���� ��� �ʼ�
}



void USHInventoryComponent::OnRep_Inventory()
{
	UE_LOG(LogTemp, Warning, TEXT("Inventory�� ����Ǿ����ϴ�!"));
	FString Message = FString::Printf(TEXT("�κ��丮�� �������� ����Ǿ����� ���ø�����Ʈ �Լ� ����"));
	UKismetSystemLibrary::PrintString(this, Message);
	ADTS_GamePlayPlayerController* CurrentOwner = Cast<ADTS_GamePlayPlayerController>(GetOwner());
	if (CurrentOwner) {
		CurrentOwner->ReLoadInventory();

	}
	else {
		Message = FString::Printf(TEXT("Controller Cast Fail"));
		UKismetSystemLibrary::PrintString(this, Message);
	}
}

void USHInventoryComponent::OnRep_QuickSlot()
{
	UE_LOG(LogTemp, Warning, TEXT("QuickSlot�� ����Ǿ����ϴ�!"));
	FString Message = FString::Printf(TEXT("QuickSlo�̰� �������� ����Ǿ����� ���ø�����Ʈ �Լ� ����"));
	UKismetSystemLibrary::PrintString(this, Message);
	ADTS_GamePlayPlayerController* CurrentOwner = Cast<ADTS_GamePlayPlayerController>(GetOwner());
	if (CurrentOwner) {
		CurrentOwner->ReLoadQuickSlot();

	}
	else {
		Message = FString::Printf(TEXT("Controller Cast Fail"));
		UKismetSystemLibrary::PrintString(this, Message);
	}
}






