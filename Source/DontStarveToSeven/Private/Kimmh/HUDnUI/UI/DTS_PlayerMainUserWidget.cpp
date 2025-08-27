// Fill out your copyright notice in the Description page of Project Settings.


#include "Kimmh/HUDnUI/UI/DTS_PlayerMainUserWidget.h"
#include "Kimmh/HUDnUI/UI/DTS_InventoryUserWidget.h"
#include "Kimmh/HUDnUI/UI/DTS_InventoryTempSlotUserWidget.h"
#include "Kimmh/HUDnUI/UI/DTS_QuickSlotUserWidget.h"
#include "Engine/AssetManager.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/SizeBoxSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kimmh/HUDnUI/UI/DTS_EmptyUserWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kimmh/Character/Controller/DTS_GamePlayPlayerController.h"
#include "Kimmh/HUDnUI/UI/DTS_ItemInfoUserWidget.h"

UDTS_PlayerMainUserWidget::UDTS_PlayerMainUserWidget(const FObjectInitializer& ObjectInitializer)
{

	bUpdateTempSlotPositionToMouse = false;
	bIsCanOpenItemInfo = true;

	static ConstructorHelpers::FClassFinder<UDTS_InventoryUserWidget> InventoryWBP(TEXT("/Game/Kimmh/UI/Inventory/WBP_Inventory.WBP_Inventory_C"));

	if (InventoryWBP.Succeeded()) {
		InvnetoyWBPClass = InventoryWBP.Class;
	}
}

void UDTS_PlayerMainUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InventoryBorder->ClearChildren();
	CreateInventoryUserWidget();
	ClearTempSlot();
	TempSlotVisible(false);
	bUpdateTempSlotPositionToMouse = false;

	WBP_QuickSlot->OnQuickInvenSlotHovered.AddDynamic(
		this,
		&UDTS_PlayerMainUserWidget::OnSlotHovered
	);

	WBP_Empty->OnEmptySlotHovered.AddDynamic(
		this,
		&UDTS_PlayerMainUserWidget::OnSlotHovered
	);

	SetItemInfoVisible(false);
}

void UDTS_PlayerMainUserWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry,DeltaTime);
	/*if (bUpdateTempSlotPositionToMouse) {
		UpdateTempSlotPositionToMouse();
	}
	else {

	}*/
}

UDTS_InventoryUserWidget* UDTS_PlayerMainUserWidget::CreateInventoryUserWidget()
{
	/*
	TSubclassOf<UDTS_InventoryUserWidget> InventoryUserWidgetClass;

	InventoryUserWidgetClass = LoadClass<UDTS_InventoryUserWidget>(nullptr, TEXT("/Game/Kimmh/UI/WBP_Inventory.WBP_Inventory_C"));*/


	/*FSoftObjectPath WidgetPath(TEXT("/Game/Kimmh/UI/Inventory/WBP_Inventory.WBP_Inventory_C"));

	UClass* InventoryUserWidgetClass = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), nullptr, *WidgetPath.ToString()));*/

	//if (InventoryUserWidgetClass) 
	if (InvnetoyWBPClass) {
		UDTS_InventoryUserWidget* InventoryUserWidget = CreateWidget<UDTS_InventoryUserWidget>(GetWorld()->GetFirstPlayerController(), InvnetoyWBPClass);
		if (InventoryUserWidget) {
			InventoryBorder->AddChild(InventoryUserWidget);
			DTS_InvnetoryUserWidget = InventoryUserWidget;

			InventoryUserWidget->OnGirdInvenSlotHovered.AddDynamic(
				this,
				&UDTS_PlayerMainUserWidget::OnSlotHovered
			);

			return InventoryUserWidget;
		}
	}


	return nullptr;
}

void UDTS_PlayerMainUserWidget::ReLoadInventory()
{
	DTS_InvnetoryUserWidget->LoadInventory();
}

void UDTS_PlayerMainUserWidget::ReloadQuickSlot()
{
	WBP_QuickSlot->LoadQuickSlot();
}

void UDTS_PlayerMainUserWidget::SetTempSlot()
{
	if (OldSlot.SlotWidget->bItemSet) {
		// �������� ������
		TempSlotVisible(true);
		WBP_InvenTempSlot->SetItemImage(OldSlot.SlotWidget->ItemIconTexture);
		WBP_InvenTempSlot->SetItemCount(OldSlot.SlotWidget->ItemCountValue);
		bIsCanInteraction = true;
		bIsCanOpenItemInfo = false;
	}
	else {
		bIsCanInteraction = false;
	}

	//// �ӽ÷� ������ ���� �� ������ �ϸ� �ȵǴ°���
	//TempSlotVisible(true);
	//WBP_InvenTempSlot->SetItemImage(OldSlot.SlotWidget->ItemIconTexture);
	//WBP_InvenTempSlot->SetItemCount(OldSlot.SlotWidget->ItemCountValue);
	//bIsCanInteraction = true;
}

void UDTS_PlayerMainUserWidget::ClearTempSlot()
{
	WBP_InvenTempSlot->ClearItem();
}

void UDTS_PlayerMainUserWidget::TempSlotVisible(bool bIsTempSlotVisible)
{
	if (bIsTempSlotVisible) {
		WBP_InvenTempSlot->SetVisibility(ESlateVisibility::Visible);
		bUpdateTempSlotPositionToMouse = true;
	}
	else {
		WBP_InvenTempSlot->SetVisibility(ESlateVisibility::Collapsed);
		bUpdateTempSlotPositionToMouse = false;
	}
	
}

void UDTS_PlayerMainUserWidget::SetItemInfoVisible(bool bIsVisible)
{
	if (bIsVisible) {
		WBP_ItemInfo->OnVisible();
	}
	else {
		WBP_ItemInfo->OnCollapsed();
	}
}


void UDTS_PlayerMainUserWidget::UpdateTempSlotPositionToMouse()
{
	FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
	FVector2D TempSlotPosition = FVector2D(MousePosition.X + 5, MousePosition.Y - 100);
	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(WBP_InvenTempSlot->Slot);
	if (CanvasSlot) {
		CanvasSlot->SetPosition(TempSlotPosition);
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("ĵ���� �г� �������� ��ȯ �Ұ�"));
	}
}

void UDTS_PlayerMainUserWidget::UpdateItemInfoPositionToMouse()
{
	FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
	FVector2D TempSlotPosition = FVector2D(MousePosition.X + 5, MousePosition.Y - 100);
	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(WBP_ItemInfo->Slot);
	if (CanvasSlot) {
		CanvasSlot->SetPosition(TempSlotPosition);
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("ĵ���� �г� �������� ��ȯ �Ұ�"));
	}
}

void UDTS_PlayerMainUserWidget::OnSlotHovered(int Row, int Col, EInvenSlot SlotKind, UDTS_InvenSlotUserWidget* SlotWidget)
{
	FString EnumName = UEnum::GetValueAsString(SlotKind);
	FString Message = EnumName + FString::Printf(TEXT(" Row : %d, Col : %d Hovered"), Row, Col);
	UKismetSystemLibrary::PrintString(this, Message, true, true, FLinearColor(0.0f, 0.66f, 1.0f), 0.0f);
	CurrentSlot.Col = Col;
	CurrentSlot.Row = Row;
	CurrentSlot.SlotKind = SlotKind;
	CurrentSlot.SlotWidget = SlotWidget;
	if (SlotKind != EInvenSlot::EmptySlot && bIsCanOpenItemInfo && SlotWidget->bItemSet) {
		WBP_ItemInfo->SetItem(CurrentSlot.SlotWidget->ItemName);
		SetItemInfoVisible(true);
	}
	else {
		SetItemInfoVisible(false);
	}
}

FReply UDTS_PlayerMainUserWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	OldSlot = CurrentSlot;
	if (CurrentSlot.SlotWidget) {
		if (CurrentSlot.SlotWidget->bItemSet) {
			

			switch (CurrentSlot.SlotKind)
			{
			case EInvenSlot::InventorySlot:
				SetTempSlot();
				break;
			case EInvenSlot::QuickSlot:
				SetTempSlot();
				break;
			case EInvenSlot::BoxSlot:
				SetTempSlot();
				break;
			case EInvenSlot::RootSlot:
				SetTempSlot();
				break;
			case EInvenSlot::EquipSlot:
				SetTempSlot();
				break;
			case EInvenSlot::EmptySlot:
				bIsCanInteraction = false;
				bIsCanOpenItemInfo = true;
				break;
			default:
				bIsCanInteraction = false;
				bIsCanOpenItemInfo = true;
				break;
			}
		}
		else {
			bIsCanInteraction = false;
			bIsCanOpenItemInfo = true;
		}
	}
	else {
		bIsCanInteraction = false;
		bIsCanOpenItemInfo = true;
	}
	

	return FReply::Handled();
}

FReply UDTS_PlayerMainUserWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	TempSlotVisible(false);
	FString OldSlotEnumName = UEnum::GetValueAsString(OldSlot.SlotKind);
	FString CurrentSlotEnumName = UEnum::GetValueAsString(CurrentSlot.SlotKind);
	FString Message;

	ADTS_GamePlayPlayerController* CurrentPlayer = Cast<ADTS_GamePlayPlayerController>(GetOwningPlayer());

	/*if (!OldSlot.SlotWidget || !CurrentSlot.SlotWidget) {
		return FReply::Handled();
	}*/
	FInventory OriginItem;
	FInventory TargetItem;
	if (OldSlot.SlotWidget) {
		OriginItem = FInventory(true, FCoordinate(true, OldSlot.Col, OldSlot.Row), OldSlot.SlotWidget->ItemName, OldSlot.SlotWidget->ItemCountValue);
	}
	
	if (CurrentSlot.SlotWidget) {
		TargetItem = FInventory(true, FCoordinate(true, CurrentSlot.Col, CurrentSlot.Row), CurrentSlot.SlotWidget->ItemName, CurrentSlot.SlotWidget->ItemCountValue);
	}

	if (bIsCanInteraction) {
		switch (OldSlot.SlotKind)
		{
		case EInvenSlot::InventorySlot:
			switch (CurrentSlot.SlotKind)
			{
			case EInvenSlot::InventorySlot:
				// �κ��丮 -> �κ��丮�� ������ �ű�� ���� �ʿ�
				Message = FString::Printf(TEXT("�κ��丮���� �κ��丮�� �巹�� ��"));
				if (CurrentPlayer) {
					CurrentPlayer->ServerSwitchItem(OriginItem, EInventoryType::Inventory, TargetItem, EInventoryType::Inventory);
				}
				break;
			case EInvenSlot::QuickSlot:
				// �κ��丮 -> ���������� ������ �ű�� ���� �ʿ�
				Message = FString::Printf(TEXT("�κ��丮���� ���������� �巹�� ��"));
				if (CurrentPlayer) {
					CurrentPlayer->ServerSwitchItem(OriginItem, EInventoryType::Inventory, TargetItem, EInventoryType::QuickSlot);
				}

				break;
			case EInvenSlot::BoxSlot:
				// �κ��丮 -> â�� �κ��丮�� ������ �ű�� ���� �ʿ�
				Message = FString::Printf(TEXT("�κ��丮���� â��� �巹�� ��"));
				break;
			case EInvenSlot::RootSlot:
				// �׳� �����ְ� �״�� �� �κ��丮 ��ȭ ����
				Message = FString::Printf(TEXT("�κ��丮���� ���� â���� �巹�� ��"));
				break;
			case EInvenSlot::EquipSlot:
				// �κ��丮���� ��� �������� �̵��ϴ� ���� ó��
				Message = FString::Printf(TEXT("�κ��丮���� ��� â���� �巹�� ��"));
				break;
			case EInvenSlot::EmptySlot:
				// �κ��丮���� �������� �����ų� �ƹ� �ϵ� ����� ��(��ȹ�� ���� �޶���)
				Message = FString::Printf(TEXT("�κ��丮���� ������� �巹�� ��"));
				if (CurrentPlayer) {
					CurrentPlayer->ServerSpawnItemByItemName(OriginItem.ItemName, true);
					CurrentPlayer->ServerRemoveItem(OriginItem, EInventoryType::Inventory);
				}
				break;
			default:
				// �̷� ���� ����� ��
				break;
			}
			break;
		case EInvenSlot::QuickSlot:
			switch (CurrentSlot.SlotKind)
			{
			case EInvenSlot::InventorySlot:
				// ������ -> �κ��丮�� ������ �ű�� ���� �ʿ�
				Message = FString::Printf(TEXT("�����Կ��� �κ��丮�� �巹�� ��"));
				if (CurrentPlayer) {
					CurrentPlayer->ServerSwitchItem(OriginItem, EInventoryType::QuickSlot, TargetItem, EInventoryType::Inventory);
				}
				break;
			case EInvenSlot::QuickSlot:
				// ������ -> ���������� ������ �ű�� ���� �ʿ�
				Message = FString::Printf(TEXT("�����Կ��� ���������� �巹�� ��"));
				if (CurrentPlayer) {
					CurrentPlayer->ServerSwitchItem(OriginItem, EInventoryType::QuickSlot, TargetItem, EInventoryType::QuickSlot);
				}
				break;
			case EInvenSlot::BoxSlot:
				// ������ -> â�� �κ��丮�� ������ �ű�� ���� �ʿ�
				Message = FString::Printf(TEXT("�����Կ��� â��� �巹�� ��"));
				break;
			case EInvenSlot::RootSlot:
				// �׳� �����ְ� �״�� �� �κ��丮 ��ȭ ����
				Message = FString::Printf(TEXT("�����Կ��� ���� â���� �巹�� ��"));
				break;
			case EInvenSlot::EquipSlot:
				// �����Կ��� ��� �������� �̵��ϴ� ���� ó��
				Message = FString::Printf(TEXT("�����Կ��� ��� â���� �巹�� ��"));
				break;
			case EInvenSlot::EmptySlot:
				// �����Կ��� �������� �����ų� �ƹ� �ϵ� ����� ��(��ȹ�� ���� �޶���)
				Message = FString::Printf(TEXT("�����Կ��� ������� �巹�� ��"));
				if (CurrentPlayer) {
					CurrentPlayer->ServerRemoveItem(OriginItem, EInventoryType::QuickSlot);
					CurrentPlayer->ServerSpawnItemByItemName(OriginItem.ItemName, true);
				}
				break;
			default:
				// �̷� ���� ����� ��
				break;
			}
			break;
		case EInvenSlot::BoxSlot:
			break;
		case EInvenSlot::RootSlot:
			break;
		case EInvenSlot::EquipSlot:
			break;
		case EInvenSlot::EmptySlot:
			break;
		default:
			break;
		}
		
		UKismetSystemLibrary::PrintString(this, Message, true, true, FLinearColor(0.0f, 0.66f, 1.0f), 5.f);

		Message =
			OldSlotEnumName
			+ FString::Printf(TEXT(" Row : %d, Col : %d ->"), OldSlot.Row, OldSlot.Col)
			+ CurrentSlotEnumName
			+ FString::Printf(TEXT(" Row : %d, Col : %d ->"), CurrentSlot.Row, CurrentSlot.Col);
		UKismetSystemLibrary::PrintString(this, Message, true, true, FLinearColor(0.0f, 0.66f, 1.0f), 5.f);
	}
	bIsCanOpenItemInfo = true;
	
	return FReply::Handled();
}

FReply UDTS_PlayerMainUserWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	UpdateTempSlotPositionToMouse();
	UpdateItemInfoPositionToMouse();
	return FReply::Handled();
}



