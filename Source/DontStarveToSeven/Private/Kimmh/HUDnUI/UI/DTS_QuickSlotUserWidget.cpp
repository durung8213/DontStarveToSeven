// Fill out your copyright notice in the Description page of Project Settings.


#include "Kimmh/HUDnUI/UI/DTS_QuickSlotUserWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Kimmh/Enum/InvenSlotEnum.h"
#include "Kimmh/HUDnUI/UI/DTS_InvenSlotUserWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kimmh/Character/Controller/DTS_GamePlayPlayerController.h"
#include "Leesh/SHInventoryComponent.h"

void UDTS_QuickSlotUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	QuickSlotHorizontalBox->ClearChildren();
	CreateQuickSlot();
	ClearAllQuickSlot();
	LoadQuickSlot();
}


void UDTS_QuickSlotUserWidget::CreateQuickSlot()
{
	//TSubclassOf<UDTS_InvenSlotUserWidget> InvenSlotUserWidgetClass;

	//InvenSlotUserWidgetClass = LoadClass<UDTS_InvenSlotUserWidget>(nullptr, TEXT("/Game/Kimmh/UI/WBP_InvenSlot.WBP_InvenSlot_C"));
	FSoftObjectPath WidgetPath(TEXT("/Game/Kimmh/UI/Inventory/WBP_InvenSlot.WBP_InvenSlot_C"));

	UClass* InvenSlotUserWidgetClass = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), nullptr, *WidgetPath.ToString()));

	for (int QuickSlotSize = 0; QuickSlotSize < 6; QuickSlotSize++)
	{
		UDTS_InvenSlotUserWidget* InvenSlotUserWidget = CreateWidget<UDTS_InvenSlotUserWidget>(GetWorld()->GetFirstPlayerController(), InvenSlotUserWidgetClass);
		if (InvenSlotUserWidget) {
			InvenSlotUserWidget->Row = 0;
			InvenSlotUserWidget->Col = QuickSlotSize;
			InvenSlotUserWidget->SlotKind = EInvenSlot::QuickSlot;

			InvenSlotUserWidget->OnInvenSlotHovered.AddDynamic(
				this,
				&UDTS_QuickSlotUserWidget::SlotHovered
			);
			QuickSlotArr.Add(InvenSlotUserWidget);

			//QuickSlotHorizontalBox->AddChildToHorizontalBox(InvenSlotUserWidget);
			UHorizontalBoxSlot* BoxSlot = Cast<UHorizontalBoxSlot>(QuickSlotHorizontalBox->AddChild(InvenSlotUserWidget));
			if (BoxSlot)
			{
				BoxSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));

				//BoxSlot->SetHorizontalAlignment(HAlign_Fill);
				//BoxSlot->SetPadding(FMargin(5.0f)); // 여백 추가 (선택 사항)
			}
		}
		else {
			UE_LOG(LogTemp, Log, TEXT("인벤 슬롯 생성 불가"));
		}
	}

}

void UDTS_QuickSlotUserWidget::ClearAllQuickSlot()
{
	for (UDTS_InvenSlotUserWidget* QuickSlot : QuickSlotArr) {
		QuickSlot->ClearItem();
		QuickSlot->bItemSet = false;
	}
}

void UDTS_QuickSlotUserWidget::LoadQuickSlot()
{
	ClearAllQuickSlot();
	UDataTable* DataTable;		// 데이터 테이블 변수 선언
	DataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Leesh/Table/DT_TestItemInfo.DT_TestItemInfo"));	// 데이터 테이블 불러옴

	FString Message;

	if (ADTS_GamePlayPlayerController* CurrentController = Cast<ADTS_GamePlayPlayerController>(GetOwningPlayer())) {
		// 컨트롤러를 CurrentController로 변환이 가능하면
		TArray<FInventory> CurrentQuickSlot = CurrentController->InventoryComponent->QuickSlot;	// 퀵슬롯 배열 가져옴

		Message = FString::Printf(TEXT("퀵슬롯 0번칸에 있는 아이템 이름 : ")) + CurrentQuickSlot[0].ItemName;
		UKismetSystemLibrary::PrintString(this, Message, true, true, FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));

		for (int ItemNum = 0; ItemNum < CurrentQuickSlot.Num(); ++ItemNum) {
			if (CurrentQuickSlot[ItemNum].InventoryCoordinate.FillSlot) {

				Message = FString::Printf(TEXT("QuickSlot %d Slot Not Empty"));
				UKismetSystemLibrary::PrintString(this, Message, true, true, FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));

				FTestItemInfo* Row = DataTable->FindRow<FTestItemInfo>(FName(*CurrentQuickSlot[ItemNum].ItemName), TEXT("General"));
				if (Row) {
					if (Row->ItemIcon) {
						QuickSlotArr[ItemNum]->SetItemImage(Row->ItemIcon.Get());
					}
					else {
						QuickSlotArr[ItemNum]->SetItemImage(Row->ItemIcon.LoadSynchronous());
					}
					QuickSlotArr[ItemNum]->SetItemCount(CurrentQuickSlot[ItemNum].ItemCount);
					QuickSlotArr[ItemNum]->ItemName = CurrentQuickSlot[ItemNum].ItemName;
					QuickSlotArr[ItemNum]->bItemSet = true;
				}
				else {
					Message = FString::Printf(TEXT("데이터 테이블에서 Row값을 가져오는데 실패하였습니다, 가져올려고 하는 Row값 : ")) + CurrentQuickSlot[ItemNum].ItemName;
					UKismetSystemLibrary::PrintString(this, Message, true, true, FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));
				}

			}
			else {
				if (QuickSlotArr.IsValidIndex(ItemNum)) {
					QuickSlotArr[ItemNum]->bItemSet = false;

				}
				Message = FString::Printf(TEXT("QuickSlot %d Slot Empty"), ItemNum);
				UKismetSystemLibrary::PrintString(this, Message, true, true, FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));
			}

		}
	}
	else {
		Message = FString::Printf(TEXT("PlayerController Call Fail"));
		UKismetSystemLibrary::PrintString(this, Message, true, true, FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));
	}
}

void UDTS_QuickSlotUserWidget::SlotHovered(int Row, int Col, EInvenSlot SlotKind, UDTS_InvenSlotUserWidget* SlotWidget)
{
	OnQuickInvenSlotHovered.Broadcast(Row, Col, SlotKind, SlotWidget);
}

void UDTS_QuickSlotUserWidget::SelectQuickSlot(int SlotNum)
{
	for (UDTS_InvenSlotUserWidget* QuickSlot : QuickSlotArr) {
		QuickSlot->QuickSlotNotSelect();
	}

	if (SlotNum >= 0 && SlotNum < QuickSlotArr.Num()) {
		QuickSlotArr[SlotNum]->QuickSlotSelect();
	}

	FString Message = FString::Printf(TEXT("QuickSlot : %d, "), SlotNum);
	UKismetSystemLibrary::PrintString(this, Message, true, true, FLinearColor(0.0f, 0.66f, 1.0f), 2.0f);
}
