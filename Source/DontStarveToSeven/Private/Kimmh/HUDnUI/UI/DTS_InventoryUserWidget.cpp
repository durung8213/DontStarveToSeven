// Fill out your copyright notice in the Description page of Project Settings.


#include "Kimmh/HUDnUI/UI/DTS_InventoryUserWidget.h"
#include "Components/GridPanel.h"
#include "Kimmh/HUDnUI/UI/DTS_InvenSlotUserWidget.h"
#include "Kimmh/Character/Controller/DTS_GamePlayPlayerController.h"
#include "Leesh/SHInventoryComponent.h"
#include "Leesh/TestItemStruct.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kimmh/Enum/InvenSlotEnum.h"

UDTS_InventoryUserWidget::UDTS_InventoryUserWidget(const FObjectInitializer& ObjectInitializer)
{

}

void UDTS_InventoryUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InvenGrid->ClearChildren();
	CreateInvenSlot();
	LoadInventory();
}

void UDTS_InventoryUserWidget::CreateInvenSlot()
{
	/*
	TSubclassOf<UDTS_InvenSlotUserWidget> InvenSlotUserWidgetClass;

	InvenSlotUserWidgetClass = LoadClass<UDTS_InvenSlotUserWidget>(nullptr, TEXT("/Game/Kimmh/UI/WBP_InvenSlot.WBP_InvenSlot_C"));*/

	FSoftObjectPath WidgetPath(TEXT("/Game/Kimmh/UI/Inventory/WBP_InvenSlot.WBP_InvenSlot_C"));

	UClass* InvenSlotUserWidgetClass = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), nullptr, *WidgetPath.ToString()));

	if (InvenSlotUserWidgetClass) {
		for (int row = 0; row < InvnetoryRow; ++row) {
			for (int col = 0; col < InvnetoryCol; ++col) {
				UDTS_InvenSlotUserWidget* InvenSlotUserWidget = CreateWidget<UDTS_InvenSlotUserWidget>(GetWorld()->GetFirstPlayerController(), InvenSlotUserWidgetClass);
				if (InvenSlotUserWidget) {
					InvenSlotUserWidget->Row = row;
					InvenSlotUserWidget->Col = col;
					InvenSlotUserWidget->SlotKind = EInvenSlot::InventorySlot;
					InvenSlotUserWidget->ClearItem();

					InvenSlotUserWidget->OnInvenSlotHovered.AddDynamic(
						this,
						&UDTS_InventoryUserWidget::SlotHovered
					);

					InvenSlotArr.Add(InvenSlotUserWidget);
					InvenGrid->AddChildToGrid(InvenSlotUserWidget, row, col);
				}
				else {
					UE_LOG(LogTemp, Log, TEXT("인벤 슬롯 생성 불가"));
				}
			}
		}
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("인벤 슬롯 WBP 불러오기 실패"));
	}
}

void UDTS_InventoryUserWidget::ClearAllInvenSlot() 
{
	for (UDTS_InvenSlotUserWidget* InvenSlot : InvenSlotArr) 
	{
		InvenSlot->ClearItem();
		InvenSlot->bItemSet = false;
	}
}

void UDTS_InventoryUserWidget::LoadInventory()
{
	ClearAllInvenSlot();
	UDataTable* DataTable;		// 데이터 테이블 변수 선언
	DataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Leesh/Table/DT_TestItemInfo.DT_TestItemInfo"));	// 데이터 테이블 불러옴

	FString Message;

	if (ADTS_GamePlayPlayerController* CurrentController = Cast<ADTS_GamePlayPlayerController>(GetOwningPlayer())) {
		TArray<FInventory> CurrentInventory = CurrentController->InventoryComponent->Inventory;

		Message = FString::Printf(TEXT("0번칸에 있는 아이템 이름 : ")) + CurrentInventory[0].ItemName;
		UKismetSystemLibrary::PrintString(this, Message, true, true, FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));

		for (int ItemNum = 0; ItemNum < CurrentInventory.Num(); ++ItemNum) {
			if (CurrentInventory[ItemNum].InventoryCoordinate.FillSlot) {

				Message = FString::Printf(TEXT("인벤토리 칸에 무언가 있음"));
				UKismetSystemLibrary::PrintString(this, Message, true, true, FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));

				FTestItemInfo* Row = DataTable->FindRow<FTestItemInfo>(FName(*CurrentInventory[ItemNum].ItemName), TEXT("General"));
				if (Row) {
					if (Row->ItemIcon) {
						InvenSlotArr[CurrentInventory[ItemNum].InventoryCoordinate.X + CurrentInventory[ItemNum].InventoryCoordinate.Y * 4]->SetItemImage(Row->ItemIcon.Get());
					}
					else {
						InvenSlotArr[CurrentInventory[ItemNum].InventoryCoordinate.X + CurrentInventory[ItemNum].InventoryCoordinate.Y * 4]->SetItemImage(Row->ItemIcon.LoadSynchronous());
					}
					InvenSlotArr[CurrentInventory[ItemNum].InventoryCoordinate.X + CurrentInventory[ItemNum].InventoryCoordinate.Y * 4]->SetItemCount(CurrentInventory[ItemNum].ItemCount);
					InvenSlotArr[CurrentInventory[ItemNum].InventoryCoordinate.X + CurrentInventory[ItemNum].InventoryCoordinate.Y * 4]->ItemName = CurrentInventory[ItemNum].ItemName;
					InvenSlotArr[CurrentInventory[ItemNum].InventoryCoordinate.X + CurrentInventory[ItemNum].InventoryCoordinate.Y * 4]->bItemSet = true;
				}
				else {
					Message = FString::Printf(TEXT("데이터 테이블에서 Row값을 가져오는데 실패하였습니다, 가져올려고 하는 Row값 : ")) + CurrentInventory[ItemNum].ItemName;
					UKismetSystemLibrary::PrintString(this, Message, true, true, FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));
				}
				
			}
			else {
				InvenSlotArr[CurrentInventory[ItemNum].InventoryCoordinate.X + CurrentInventory[ItemNum].InventoryCoordinate.Y * 4]->bItemSet = false;
				Message = FString::Printf(TEXT("인벤토리 칸이 비어있음"));
				UKismetSystemLibrary::PrintString(this, Message, true, true, FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));
			}
			
		}
	}
	else {
		Message = FString::Printf(TEXT("인벤토리가 있는 플레이어 컨트롤러 가져오기 실패"));
		UKismetSystemLibrary::PrintString(this, Message, true, true, FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));
	}
}

void UDTS_InventoryUserWidget::SlotHovered(int Row, int Col, EInvenSlot SlotKind, UDTS_InvenSlotUserWidget* SlotWidget)
{
	OnGirdInvenSlotHovered.Broadcast(Row, Col, SlotKind, SlotWidget);
}









