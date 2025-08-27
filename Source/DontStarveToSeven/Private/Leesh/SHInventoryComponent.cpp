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

	

	// 인벤토리 크기 세팅.
	Inventory.SetNum(USHInventoryComponent::InventorySize);
	UE_LOG(LogTemp, Warning, TEXT("Inventory array size: %d"), Inventory.Num());


	// 인벤토리 빈슬롯,좌표 초기화
	if (Inventory.Num() >= USHInventoryComponent::InventorySize) // 안전성 체크
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


	// 퀵슬롯 크기 세팅.
	QuickSlot.SetNum(USHInventoryComponent::QuickSlotSize);
	UE_LOG(LogTemp, Warning, TEXT("QuickSlot array size: %d"), QuickSlot.Num());

	// 퀵슬롯 빈슬롯,좌표 초기화
	if (QuickSlot.Num() >= USHInventoryComponent::QuickSlotSize) // 안전성 체크
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
	// InventoryType이 None이면 유효하지 않은 타입으로 처리
	if (InventoryType == EInventoryType::None)
	{
		UE_LOG(LogTemp, Error, TEXT("[RemoveItem] Invalid InventoryType: None"));
		return;
	}
	
	// 아이템 데이터 테이블이 없을 때. 비정상 동작.
	if (!ItemDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("ItemDataTable is null"));
		return;
	}

	// 테이블에서 입력 받은 아이템 찾기.
	const FName RowName(*InItem.ItemName);
	const FTestItemInfo* ItemInfo = ItemDataTable->FindRow<FTestItemInfo>(RowName, TEXT(""));

	// 테이블에 없는 아이템일 때. 비정상 동작.
	if (!ItemInfo)
	{
		UE_LOG(LogTemp, Error, TEXT("Item not found in DataTable: %s"), *InItem.ItemName);
		return;
	}

	const int32 MaxCount = ItemInfo->ItemMaxCount; // 테이블의 MaxCount를 변수로 선언.
	int32 RemainingCount = InItem.ItemCount; // 입력 받은 아이템 개수를 변수로 선언.

	&QuickSlot;
	TArray<FInventory>* TargetArray = GetInventoryArrayByType(InventoryType); // 추가되는 인벤토리 타입을 변수로 선언.
	if (!TargetArray)
	{
		UE_LOG(LogTemp, Error, TEXT("[AddItem] Invalid inventory type"));
		return;
	}

	// 1. 좌표 기반일 경우, 정확한 위치에 덮어쓰기
	if (InItem.bUseCoord)
	{
		// 입력 받은 좌표의 인덱스 찾기
		int32 Index = TargetArray->IndexOfByPredicate([&](const FInventory& Slot)
			{
				return Slot.InventoryCoordinate == InItem.InventoryCoordinate;
			});

		if (TargetArray->IsValidIndex(Index))
		{
			FInventory& Slot = (*TargetArray)[Index];

			// 1-1. 기존에 같은 아이템이면 Count 추가
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
				// 1-2. 다른 아이템이 이미 있는 경우 -> 덮어쓰기 불가
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
	// 2. 좌표 미지정 자동 배치
	if (!InItem.bUseCoord && RemainingCount > 0)
	{
		// 2-1. 먼저 기존 슬롯에 같은 아이템 있는 경우 -> 스택 추가
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
		// 2-2. 남은 수량을 빈 슬롯에 새로 채우기
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

	// Inventory 배열을 순회하면서, 빈슬롯 검사하여 반환.
	for (int32 Index = 0; Index < InArray.Num(); ++Index)
	{
		if (!InArray[Index].InventoryCoordinate.FillSlot)
		{
			UE_LOG(LogTemp, Warning, TEXT("FindIndexNum: %s"), *FString::FromInt(Index));
			UE_LOG(LogTemp, Warning, TEXT("TempSlot item: (%d , %d)"), InArray[Index].InventoryCoordinate.X, InArray[Index].InventoryCoordinate.Y);
			return Index;
		}

	}
	// 빈 슬롯이 없는 경우 -1 반환
	return -1;
}

void USHInventoryComponent::RemoveItem(const FInventory& InItem, EInventoryType InventoryType)
{
	// InventoryType이 None이면 유효하지 않은 타입으로 처리
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
	

	// 1. 좌표 기반 제거
	if (InItem.bUseCoord)
	{
		// 입력된 좌표 기준으로 인덱스 찾기.
		int32 TargetIndex = TargetArray->IndexOfByPredicate([&](const FInventory& Slot)
		{
				return Slot.InventoryCoordinate == InItem.InventoryCoordinate;
		});

		// 제거하려는 좌표가 없는 좌표일 때(잘못된 좌표 입력)
		if (!TargetArray->IsValidIndex(TargetIndex))
		{
			UE_LOG(LogTemp, Error, TEXT("[RemoveItem Coord] Invalid coord (%d,%d)"),
				InItem.InventoryCoordinate.X, InItem.InventoryCoordinate.Y);
			return;
		}
		// 제거하려는 아이템명이 없을 때.
		FInventory& Slot = (*TargetArray)[TargetIndex];
		if (Slot.ItemName != InItem.ItemName)
		{
			UE_LOG(LogTemp, Error, TEXT("[RemoveItem Coord] Item mismatch at (%d,%d): %s vs %s"),
				Slot.InventoryCoordinate.X, Slot.InventoryCoordinate.Y,
				*Slot.ItemName, *InItem.ItemName);
			return;
		}
		// 기존에 아이템 개수가 모자랄 때.
		if (Slot.ItemCount < RemainingToRemove)
		{
			UE_LOG(LogTemp, Warning, TEXT("[RemoveItem Coord] Not enough items to remove."));
			return;
		}

		// 정상 제거.
		Slot.ItemCount -= RemainingToRemove;

		// 제거 후 처리.
		if (Slot.ItemCount == 0)
		{
			Slot = FInventory(true, Slot.InventoryCoordinate, TEXT(""), 0);
			Slot.InventoryCoordinate.FillSlot = false;
		}

		UE_LOG(LogTemp, Warning, TEXT("[RemoveItem Coord] Removed %d from (%d,%d)"),
			RemainingToRemove, Slot.InventoryCoordinate.X, Slot.InventoryCoordinate.Y);
		return;
	}

	// 2. 아이템명 기반 전체 제거 (여러 슬롯 분산 스택 제거)
	for (FInventory& Slot : *TargetArray)
	{
		// 삭제할 개수가 남지 않았다면 루프 탈출.
		if (RemainingToRemove <= 0)
		{
			break;
		}
		// 전체 인벤토리에서 삭제하려는 아이템명으로 찾아서 삭제.
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
	// 인벤토리 타입이 유효한지 확인 (None이면 오류 처리)
	if (OriginInventoryType == EInventoryType::None || TargetInventoryType == EInventoryType::None)
	{
		UE_LOG(LogTemp, Error, TEXT("[SwitchItem] Invalid inventory type provided. OriginType: %d, TargetType: %d"),
			(uint8)OriginInventoryType, (uint8)TargetInventoryType);
		return;
	}
	
	// 0. 인벤토리 배열 가져오기
	TArray<FInventory>* OriginArray = GetInventoryArrayByType(OriginInventoryType);
	TArray<FInventory>* TargetArray = GetInventoryArrayByType(TargetInventoryType);
	if (!OriginArray || !TargetArray)
	{
		UE_LOG(LogTemp, Error, TEXT("[SwitchItem] Invalid Origin/Target Array."));
		return;
	}

	// 1. OriginSlot, TargetSlot 인덱스 찾기
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

	// 유효성 검사
	if (!OriginArray->IsValidIndex(OriginIndex) || !TargetArray->IsValidIndex(TargetIndex))
	{
		UE_LOG(LogTemp, Error, TEXT("[SwitchItem] Invalid indices for Origin/Target."));
		return;
	}

	FInventory& OriginSlot = (*OriginArray)[OriginIndex];
	FInventory& TargetSlot = (*TargetArray)[TargetIndex];

	// 만약 아무것도 없는 슬롯을 잘못 클릭했거나, Origin 자체가 비어있으면 무시
	if (OriginSlot.ItemCount <= 0 || OriginSlot.ItemName.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("[SwitchItem] Origin is empty. Nothing to move."));
		return;
	}
	
	// 만약 마우스를 제자리에 놓는 경우 무시. (같은 자리에서 들었다가 놓는 동작.)
	if (OriginInventoryType == TargetInventoryType)
	{
		if (OriginIndex == TargetIndex)
		{
			UE_LOG(LogTemp, Warning, TEXT("[SwitchItem] Same slot selected. No switch performed."));
			return;
		}
	}


	// 아이템 데이터 테이블 조회 (스택 병합 시 필요)
	if (!ItemDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("[SwitchItem] ItemDataTable is null."));
		return;
	}

	// 2. 같은 아이템이면 스택 병합
	if (!TargetSlot.ItemName.IsEmpty() && (OriginSlot.ItemName == TargetSlot.ItemName))
	{
		// 데이터 테이블에서 아이템 정보 검색
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

		// 전체 개수가 MaxCount 이하라면 Target에 몰아넣고 Origin은 비운다
		if (TotalCount <= MaxCount)
		{
			TargetSlot.ItemCount = TotalCount;

			// Origin 슬롯 초기화
			OriginSlot.ItemName = TEXT("");
			OriginSlot.ItemCount = 0;
			OriginSlot.InventoryCoordinate.FillSlot = false;  // 슬롯 비었음을 표시
			// bUseCoord는 드래그앤드롭에서는 보통 true를 유지할 수도 있지만, 
			// 일단 아이템이 사라진 슬롯은 FillSlot=false로 처리하면 충분.

			UE_LOG(LogTemp, Warning, TEXT("[SwitchItem] Stack merged into Target (%d,%d). Total: %d"),
				TargetSlot.InventoryCoordinate.X, TargetSlot.InventoryCoordinate.Y, TargetSlot.ItemCount);
		}
		else
		{
			// TargetSlot은 MaxCount로 채우고, 나머지는 Origin에 남김
			TargetSlot.ItemCount = MaxCount;
			OriginSlot.ItemCount = TotalCount - MaxCount;

			UE_LOG(LogTemp, Warning, TEXT("[SwitchItem] Partial merge. Target:%d, Origin:%d"),
				TargetSlot.ItemCount, OriginSlot.ItemCount);
		}
		return;
	}

	// 3. Target이 빈 슬롯이면 → 단순 이동
	if (TargetSlot.ItemName.IsEmpty())
	{
		// Target 슬롯에 Origin 아이템 전부 복사
		TargetSlot = OriginSlot;
		TargetSlot.InventoryCoordinate = TargetItem.InventoryCoordinate;

		// Origin 슬롯 초기화
		OriginSlot.ItemName = TEXT("");
		OriginSlot.ItemCount = 0;
		OriginSlot.InventoryCoordinate.FillSlot = false;

		UE_LOG(LogTemp, Warning, TEXT("[SwitchItem] Moved item to empty Target slot (%d,%d)."),
			TargetSlot.InventoryCoordinate.X, TargetSlot.InventoryCoordinate.Y);

		return;
	}

	// 4. 다른 아이템이면 → 서로 교체 (Swap)
	{
		// 좌표만 서로 바꿔 준 뒤, 슬롯 내용 교환
		FCoordinate TempCoord = OriginSlot.InventoryCoordinate;
		OriginSlot.InventoryCoordinate = TargetSlot.InventoryCoordinate;
		TargetSlot.InventoryCoordinate = TempCoord;

		// 실제 아이템 정보 교환
		Swap(OriginSlot, TargetSlot);

		UE_LOG(LogTemp, Warning, TEXT("[SwitchItem] Swapped different items: Origin(%d,%d) <-> Target(%d,%d)"),
			TargetSlot.InventoryCoordinate.X, TargetSlot.InventoryCoordinate.Y,
			OriginSlot.InventoryCoordinate.X, OriginSlot.InventoryCoordinate.Y);
	}
	


}

int32 USHInventoryComponent::SearchItemCount(FString InItemName, EInventoryType InventoryType)
{
	// InventoryType이 None이면 유효하지 않은 타입으로 처리
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
	// InventoryType이 None이면 유효하지 않은 타입으로 처리
	if (InventoryType == EInventoryType::None)
	{
		UE_LOG(LogTemp, Error, TEXT("[RemoveItem] Invalid InventoryType: None"));
		return FSearchItemResult();  // 기본값 반환;
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

	DOREPLIFETIME(USHInventoryComponent, Inventory); // 변수 등록 필수
	DOREPLIFETIME(USHInventoryComponent, QuickSlot); // 변수 등록 필수
}



void USHInventoryComponent::OnRep_Inventory()
{
	UE_LOG(LogTemp, Warning, TEXT("Inventory가 변경되었습니다!"));
	FString Message = FString::Printf(TEXT("인벤토리가 서버에서 변경되었으로 리플리케이트 함수 실행"));
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
	UE_LOG(LogTemp, Warning, TEXT("QuickSlot가 변경되었습니다!"));
	FString Message = FString::Printf(TEXT("QuickSlo이가 서버에서 변경되었으로 리플리케이트 함수 실행"));
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






