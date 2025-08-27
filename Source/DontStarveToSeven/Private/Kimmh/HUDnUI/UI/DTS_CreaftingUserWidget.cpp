// Fill out your copyright notice in the Description page of Project Settings.


#include "Kimmh/HUDnUI/UI/DTS_CreaftingUserWidget.h"
#include "Kimmh/HUDnUI/UI/DTS_CreaftingGridUserWidget.h"
#include "Kimmh/HUDnUI/UI/DTS_CreaftTypeButtonUserWidget.h"
#include "Kimmh/HUDnUI/UI/DTS_CraftingInfoUserWidget.h"
#include "Kimmh/HUDnUI/UI/DTS_CraftItemCreateUserWidget.h"
#include "Kimmh/Struct/CreaftingStruct.h"
#include "Kimmh/Struct/ResourceItemStruct.h"
#include "Components/Border.h"


UDTS_CreaftingUserWidget::UDTS_CreaftingUserWidget()
{
	static ConstructorHelpers::FClassFinder<UDTS_CreaftTypeButtonUserWidget> CreaftTypeButton(TEXT("/Game/Kimmh/UI/Creafting/WBP_CreaftingTypeButton.WBP_CreaftingTypeButton_C"));
	if (CreaftTypeButton.Succeeded()) {
		CreaftingTypeButtonUserWidgetClass = CreaftTypeButton.Class;
	}

	static ConstructorHelpers::FClassFinder<UDTS_CreaftingGridUserWidget> CreaftGrid(TEXT("/Game/Kimmh/UI/Creafting/WBP_CreaftingGrid.WBP_CreaftingGrid_C"));
	if (CreaftGrid.Succeeded()) {
		CreaftingGridUserWidgetClass = CreaftGrid.Class;
	}
	
	static ConstructorHelpers::FClassFinder<UDTS_CraftingInfoUserWidget> CreaftInfo(TEXT("/Game/Kimmh/UI/Creafting/WBP_CreaftingInfo.WBP_CreaftingInfo_C"));

	if (CreaftInfo.Succeeded()) {
		CreaftingInfoUserWidgetClass = CreaftInfo.Class;
	}

	static ConstructorHelpers::FClassFinder<UDTS_CraftItemCreateUserWidget> CreaftButton(TEXT("/Game/Kimmh/UI/Creafting/WBP_RequireItemAndCreateButton.WBP_RequireItemAndCreateButton_C"));
	if (CreaftButton.Succeeded()) {
		CraftintItemCreateUserWidgetClass = CreaftButton.Class;
	}
}

void UDTS_CreaftingUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	CreaftingTypeButtonBorder->ClearChildren();	// 크래프팅 타입 버튼 보더의 자식 제거
	CreateCreaftingTypeButtonWidget();	// 크래프팅 타입 버튼 위젯 생성 및 붙여주는 함수 실행
	
	CreaftingGridBorder->ClearChildren();	// 크래프팅 그리드 보더의 자식 제거 
	CreateCreaftingGridWidget();	// 크래프팅 그리드 위젯 생성 및 붙여주는 함수 실행

	CreaftingInfoBorder->ClearChildren();	// 크래프팅 아이템 정보 보더의 자식 제거
	CreateCraftingInfo();	// 크래프팅 아이템 정보 위젯 생성 및 붙여주는 함수 실행

	CreaftingNeedItem->ClearChildren();	// 크래프팅 아이템 제작 요구 아이템 보더의 자식 제거
	CreateCraftingItemCreateWidget();	// 크래프팅 아이템 제작 요구 아이템 위젯 생성 및 붙여주는 함수 실행

}

/// <summary>
/// 크래프팅 타입 버튼 위젯 생성 및 붙여주는 함수
/// </summary>
void UDTS_CreaftingUserWidget::CreateCreaftingTypeButtonWidget()
{
	/*TSubclassOf<UDTS_CreaftTypeButtonUserWidget> CreaftingTypeButtonUserWidgetClass;
	CreaftingTypeButtonUserWidgetClass = LoadClass<UDTS_CreaftTypeButtonUserWidget>(nullptr, TEXT("/Game/Kimmh/UI/Creafting/WBP_CreaftingTypeButton.WBP_CreaftingTypeButton_C"));*/

	//FSoftObjectPath WidgetPath(TEXT("/Game/Kimmh/UI/Creafting/WBP_CreaftingTypeButton.WBP_CreaftingTypeButton_C"));
	//UClass* CreaftingTypeButtonUserWidgetClass = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), nullptr, *WidgetPath.ToString()));

	if (CreaftingTypeButtonUserWidgetClass) {
		UDTS_CreaftTypeButtonUserWidget* CreaftingTypeButtonUserWidget = CreateWidget<UDTS_CreaftTypeButtonUserWidget>(GetWorld()->GetFirstPlayerController(), CreaftingTypeButtonUserWidgetClass);
		if (CreaftingTypeButtonUserWidget) {
			CreaftingTypeButtonBorder->AddChild(CreaftingTypeButtonUserWidget);

			CreaftingTypeButtonUserWidget->OnConfirmClicked.AddDynamic
			(
				this, &UDTS_CreaftingUserWidget::ChangeCreaftingGridDT
			); // 크래프팅 타입을 변경하는 버튼을 눌렀다는 것을 알리는 UDTS_CreaftTypeButtonUserWidget의 델리게이트를 바인딩함
		}
		else {
			UE_LOG(LogTemp, Log, TEXT("CreaftingTypeButtonUserWidget 생성 실패"));
			FString Message = FString::Printf(TEXT("CreaftingTypeButtonUserWidget 생성 실패"));
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, Message);
		}
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("CreaftingTypeButtonUserWidget 불러오기 실패"));
		FString Message = FString::Printf(TEXT("CreaftingTypeButtonUserWidget 불러오기 실패"));
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, Message);
	}
}

/// <summary>
/// 크래프팅 그리드 위젯 생성 및 붙여주는 함수
/// </summary>
void UDTS_CreaftingUserWidget::CreateCreaftingGridWidget()
{
	
	/*TSubclassOf<UDTS_CreaftingGridUserWidget> CreaftingGridUserWidgetClass;

	CreaftingGridUserWidgetClass = LoadClass<UDTS_CreaftingGridUserWidget>(nullptr, TEXT("/Game/Kimmh/UI/WBP_CreaftingGrid.WBP_CreaftingGrid_C"));*/

	/*FSoftObjectPath WidgetPath(TEXT("/Game/Kimmh/UI/Creafting/WBP_CreaftingGrid.WBP_CreaftingGrid_C"));

	UClass* CreaftingGridUserWidgetClass = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), nullptr, *WidgetPath.ToString()));*/

	if (CreaftingGridUserWidgetClass) {
		CreaftingGridUserWidget = CreateWidget<UDTS_CreaftingGridUserWidget>(GetWorld()->GetFirstPlayerController(), CreaftingGridUserWidgetClass);
		if (CreaftingGridUserWidget) {
			CreaftingGridBorder->AddChild(CreaftingGridUserWidget);
			CreaftingGridUserWidget->OnGridPanelDelegate.AddDynamic
			(
				this, &UDTS_CreaftingUserWidget::ChangeCreaftingInfoUI
			);	// 생성할 아이템을 정했다는 것을 알리는 UDTS_CreaftingGridUserWidget의 델리게이트를 바인딩함
		}
		else {
			UE_LOG(LogTemp, Log, TEXT("CreaftingGridUserWidget 생성 실패"));
			FString Message = FString::Printf(TEXT("CreaftingGridUserWidget 생성 실패"));
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, Message);
		}
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("CreaftingGridUserWidgetClass 불러오기 실패"));
		FString Message = FString::Printf(TEXT("CreaftingGridUserWidgetClass 불러오기 실패"));
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, Message);
	}
}

/// <summary>
/// 크래프팅 아이템 정보 위젯 생성 및 붙여주는 함수
/// </summary>
void UDTS_CreaftingUserWidget::CreateCraftingInfo()
{
	/*
	TSubclassOf<UDTS_CraftingInfoUserWidget> CreaftingInfoUserWidgetClass;
	CreaftingInfoUserWidgetClass = LoadClass<UDTS_CraftingInfoUserWidget>(nullptr, TEXT("/Game/Kimmh/UI/Creafting/WBP_CreaftingInfo.WBP_CreaftingInfo_C"));*/

	//FSoftObjectPath WidgetPath(TEXT("/Game/Kimmh/UI/Creafting/WBP_CreaftingInfo.WBP_CreaftingInfo_C"));

	//UClass* CreaftingInfoUserWidgetClass = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), nullptr, *WidgetPath.ToString()));

	if (CreaftingInfoUserWidgetClass) {
		CraftingInfoUserWidget = CreateWidget<UDTS_CraftingInfoUserWidget>(GetWorld()->GetFirstPlayerController(), CreaftingInfoUserWidgetClass);
		if (CraftingInfoUserWidget) {
			CreaftingInfoBorder->AddChild(CraftingInfoUserWidget);
			CraftingInfoUserWidget->ClearInfo();	// 기본 정보 초기화
		}
		else {
			UE_LOG(LogTemp, Log, TEXT("CraftingInfoUserWidget 생성 실패"));
			FString Message = FString::Printf(TEXT("CraftingInfoUserWidget 생성 실패"));
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, Message);
		}
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("CreaftingInfoUserWidgetClass 불러오기 실패"));
		FString Message = FString::Printf(TEXT("CreaftingInfoUserWidgetClass 불러오기 실패"));
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, Message);
	}
}

/// <summary>
/// 크래프팅 아이템 제작 요구 아이템 위젯 생성 및 붙여주는 함수
/// </summary>
void UDTS_CreaftingUserWidget::CreateCraftingItemCreateWidget()
{
	/*TSubclassOf<UDTS_CraftItemCreateUserWidget> CraftintItemCreateUserWidgetClass;

	CraftintItemCreateUserWidgetClass = LoadClass<UDTS_CraftItemCreateUserWidget>(nullptr, TEXT("/Game/Kimmh/UI/Creafting/WBP_RequireItemAndCreateButton.WBP_RequireItemAndCreateButton_C"));*/

	//FSoftObjectPath WidgetPath(TEXT("/Game/Kimmh/UI/Creafting/WBP_RequireItemAndCreateButton.WBP_RequireItemAndCreateButton_C"));

	//UClass* CraftintItemCreateUserWidgetClass = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), nullptr, *WidgetPath.ToString()));

	if (CraftintItemCreateUserWidgetClass) {
		CraftintItemCreateUserWidget = CreateWidget<UDTS_CraftItemCreateUserWidget>(GetWorld()->GetFirstPlayerController(), CraftintItemCreateUserWidgetClass);
		if (CraftintItemCreateUserWidget) {
			CreaftingNeedItem->AddChild(CraftintItemCreateUserWidget);
			CraftintItemCreateUserWidget->CollispedCreafting();	// 선택한 아이템이 없으니 UI를 숨김
		}
		else {
			UE_LOG(LogTemp, Log, TEXT("CraftintItemCreateUserWidget 생성 실패"));
			FString Message = FString::Printf(TEXT("CraftintItemCreateUserWidget 생성 실패"));
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, Message);
		}
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("CraftintItemCreateUserWidgetClass 불러오기 실패"));
		FString Message = FString::Printf(TEXT("CraftintItemCreateUserWidgetClass 불러오기 실패"));
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, Message);
	}

}

/// <summary>
/// 크래프팅 종류가 바뀌었다는 것을 알리는 
/// UDTS_CreaftTypeButtonUserWidget의 델리게이트에 바인딩 될 함수
/// </summary>
/// <param name="CreaftingKind">바뀔 종류 Enum</param>
void UDTS_CreaftingUserWidget::ChangeCreaftingGridDT(ECreaftingKind CreaftingKind)
{
	if(CreaftingGridUserWidget)
		CreaftingGridUserWidget->CreateCreaftingSlots(CreaftingKind); // 크래프팅 그리드 UI에 크래프팅 종류가 바뀌었다고 알려줌

	FString Message = StaticEnum<ECreaftingKind>()->GetValueAsString(CreaftingKind);
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, Message);
}

/// <summary>
/// 생성할 아이템을 정했다는 것을 알리는 
/// UDTS_CreaftingGridUserWidget의 델리게이트를 바인딩함 
/// </summary>
/// <param name="CreaftingKind">아이템 분류</param>
/// <param name="RowName">아이템 이름</param>
void UDTS_CreaftingUserWidget::ChangeCreaftingInfoUI(ECreaftingKind CreaftingKind, FName RowName)
{
	FString Message = TEXT("[크래프팅 메인 위젯] 크래프팅 종류 : ") + StaticEnum<ECreaftingKind>()->GetValueAsString(CreaftingKind) + TEXT("RowName : ") + RowName.ToString();
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, Message);

	UDataTable* ItemCraftingDataTable;
	FString Message2;
	FCreafting* Row = nullptr;

	// 각 아이템 분류 별로 데이터 테이블을 가져오기
	switch (CreaftingKind)
	{
	case ECreaftingKind::Favourite:
		break;
	case ECreaftingKind::Item:
		ItemCraftingDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Kimmh/Blueprint/DT/DT_ItemsCreafting.DT_ItemsCreafting"));
		if (ItemCraftingDataTable) {
			Row = ItemCraftingDataTable->FindRow<FCreafting>(RowName, TEXT("General"));
		}
		break;
	case ECreaftingKind::Common:
		ItemCraftingDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Kimmh/Blueprint/DT/DT_CommonCreafting.DT_CommonCreafting"));
		if (ItemCraftingDataTable) {
			Row = ItemCraftingDataTable->FindRow<FCreafting>(RowName, TEXT("General"));
		}
		break;
	case ECreaftingKind::Tool:
		ItemCraftingDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Kimmh/Blueprint/DT/DT_ToolCreafting.DT_ToolCreafting"));
		if (ItemCraftingDataTable) {
			Row = ItemCraftingDataTable->FindRow<FCreafting>(RowName, TEXT("General"));
		}
		break;
	case ECreaftingKind::Weapon:
		ItemCraftingDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Kimmh/Blueprint/DT/DT_WeaponCreafting.DT_WeaponCreafting"));
		if (ItemCraftingDataTable) {
			Row = ItemCraftingDataTable->FindRow<FCreafting>(RowName, TEXT("General"));
		}
		break;
	case ECreaftingKind::Turret:
		ItemCraftingDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Kimmh/Blueprint/DT/DT_TurretCreafting.DT_TurretCreafting"));
		if (ItemCraftingDataTable) {
			Row = ItemCraftingDataTable->FindRow<FCreafting>(RowName, TEXT("General"));
		}
		break;
	case ECreaftingKind::Structures:
		ItemCraftingDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Kimmh/Blueprint/DT/DT_StructuresCreafting.DT_StructuresCreafting"));
		if (ItemCraftingDataTable) {
			Row = ItemCraftingDataTable->FindRow<FCreafting>(RowName, TEXT("General"));
		}
		break;
	default:
		Row = nullptr;
		break;
	}

	if (Row) {
		if (CraftingInfoUserWidget) {
			CraftingInfoUserWidget->
				SetItemInfo(
					Row->CreaftingImage,
					FText::FromString(Row->Name),
					FText::FromString("WORKBENCH Requrire"),
					FText::FromString(Row->Explan)
				);	// 제작 아이템 정보를 설정하는 함수 실행
		}

		if (CraftintItemCreateUserWidget) {
			CraftintItemCreateUserWidget->CollispedCreafting();
			CraftintItemCreateUserWidget->VisibleCreafting();
			CraftintItemCreateUserWidget->CraftType = Row->CreaftingType;
			CraftintItemCreateUserWidget->SetCraftStructureType(Row->CraftType);
			CraftintItemCreateUserWidget->CraftItemDBRowName = RowName;
			for (FResourceItem RequireItem : Row->ResourceItemArr) {
				CraftintItemCreateUserWidget->
					CreateRequireItemSlot
					(
						FText::FromString(RequireItem.ResourceItemName),
						RequireItem.ResoucreItemCount
					);	// 아이템을 제작하는데 필요한 아이템을 전달하는 함수 실행
			}
		}
	}
}

void UDTS_CreaftingUserWidget::ReloadRequireItemCount()
{
	CraftintItemCreateUserWidget->ReloadRequireItemCount();
}
