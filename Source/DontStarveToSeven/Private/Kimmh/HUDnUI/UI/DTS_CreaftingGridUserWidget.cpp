// Fill out your copyright notice in the Description page of Project Settings.


#include "Kimmh/HUDnUI/UI/DTS_CreaftingGridUserWidget.h"
#include "Engine/DataTable.h"
#include "Kimmh/Struct/CreaftingStruct.h"
#include "Kimmh/Struct/ResourceItemStruct.h"
#include "Components/UniformGridPanel.h"
#include "Kimmh/HUDnUI/UI/DTS_CraftingSlotUserWidget.h"
#include "Kismet/KismetSystemLibrary.h"

/// <summary>
/// 생성자
/// 데이터 테이블을 불러와 저장한다
/// </summary>
/// <param name="ObjectInitializer"></param>
UDTS_CreaftingGridUserWidget::UDTS_CreaftingGridUserWidget(const FObjectInitializer& ObjectInitializer)
{
	// item 데이터 테이블 불러오기
	static ConstructorHelpers::FObjectFinder<UDataTable> CraftingItemTableObject(TEXT("/Game/Kimmh/Blueprint/DT/DT_ItemsCreafting.DT_ItemsCreafting"));
	if (CraftingItemTableObject.Succeeded()) {
		ItemCraftingDataTable = CraftingItemTableObject.Object;
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("ItemCraftingDataTable 불러오기 실패"));
		FString Message = FString::Printf(TEXT("ItemCraftingDataTable 불러오기 실패"));
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, Message);
	}

	// common 데이터 테이블 불러오기
	static ConstructorHelpers::FObjectFinder<UDataTable>
		CraftingCommonTableObject(TEXT("/Game/Kimmh/Blueprint/DT/DT_CommonCreafting.DT_CommonCreafting"));
	if (CraftingCommonTableObject.Succeeded()) {
		CommonCraftingDataTable = CraftingCommonTableObject.Object;
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("CommonCraftingDataTable 불러오기 실패"));
		FString Message = FString::Printf(TEXT("CommonCraftingDataTable 불러오기 실패"));
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, Message);
	}

	// Tool 데이터 테이블 불러오기
	static ConstructorHelpers::FObjectFinder<UDataTable>
		CraftingToolTableObject(TEXT("/Game/Kimmh/Blueprint/DT/DT_ToolCreafting.DT_ToolCreafting"));
	if (CraftingToolTableObject.Succeeded()) {
		ToolCraftingDataTable = CraftingToolTableObject.Object;
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("ToolCraftingDataTable 불러오기 실패"));
		FString Message = FString::Printf(TEXT("ToolCraftingDataTable 불러오기 실패"));
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, Message);
	}

	// Weapon 데이터 테이블 불러오기
	static ConstructorHelpers::FObjectFinder<UDataTable>
		CraftingWeaponTableObject(TEXT("/Game/Kimmh/Blueprint/DT/DT_WeaponCreafting.DT_WeaponCreafting"));
	if (CraftingWeaponTableObject.Succeeded()) {
		WeaponCraftingDataTable = CraftingWeaponTableObject.Object;
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("WeaponCraftingDataTable 불러오기 실패"));
		FString Message = FString::Printf(TEXT("WeaponCraftingDataTable 불러오기 실패"));
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, Message);
	}

	// 터렛 데이터 테이블 불러오기
	static ConstructorHelpers::FObjectFinder<UDataTable>
		CraftingTurretTableObject(TEXT("/Game/Kimmh/Blueprint/DT/DT_TurretCreafting.DT_TurretCreafting"));
	if (CraftingTurretTableObject.Succeeded()) {
		TurretCraftingDataTable = CraftingTurretTableObject.Object;
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("TurretCraftingDataTable 불러오기 실패"));
		FString Message = FString::Printf(TEXT("TurretCraftingDataTable 불러오기 실패"));
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, Message);
	}

	// 구조물 데이터 테이블 불러오기
	static ConstructorHelpers::FObjectFinder<UDataTable>
		CraftingStructuresTableObject(TEXT("/Game/Kimmh/Blueprint/DT/DT_StructuresCreafting.DT_StructuresCreafting"));
	if (CraftingStructuresTableObject.Succeeded()) {
		StructuresCraftingDataTable = CraftingStructuresTableObject.Object;
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("StructuresCraftingDataTable 불러오기 실패"));
		FString Message = FString::Printf(TEXT("StructuresCraftingDataTable 불러오기 실패"));
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, Message);
	}
}

/// <summary>
/// UI가 AddToViewport가 되면 CreateCreaftingSlots 실행한다
/// </summary>
void UDTS_CreaftingGridUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	CreateCreaftingSlots(CurrentKind);
}

/// <summary>
/// Kind 맞는 크래프팅 버튼 슬롯들의 정보를 데이터 테이블에서 얻고 만드는 함수를 실행한다
/// </summary>
/// <param name="Kind">크래프팅 분류</param>
void UDTS_CreaftingGridUserWidget::CreateCreaftingSlots(ECreaftingKind Kind)
{
	/*FString Message = StaticEnum<ECreaftingKind>()->GetValueAsString(Kind);
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, Message);*/

	if(CreaftingGridPanel)
		CreaftingGridPanel->ClearChildren();

	TArray<FName> RowNames;

	switch (Kind)
	{
	case ECreaftingKind::Favourite:
		break;
	case ECreaftingKind::Item:
		break;
	case ECreaftingKind::Common:
		break;
	case ECreaftingKind::Tool:
		break;
	case ECreaftingKind::Weapon:
		RowNames = WeaponCraftingDataTable->GetRowNames();
		for (int SlotNum = 0; SlotNum < RowNames.Num(); SlotNum++) {
			FCreafting* Row = WeaponCraftingDataTable->FindRow<FCreafting>(RowNames[SlotNum], TEXT("General"));
			CreateCraftingSlot(RowNames[SlotNum], ECreaftingKind::Weapon, Row->CreaftingImage, SlotNum);
		}
		break;
	case ECreaftingKind::Turret:
		RowNames = TurretCraftingDataTable->GetRowNames();
		for (int SlotNum = 0; SlotNum < RowNames.Num(); SlotNum++) {
			FCreafting* Row = TurretCraftingDataTable->FindRow<FCreafting>(RowNames[SlotNum], TEXT("General"));
			CreateCraftingSlot(RowNames[SlotNum], ECreaftingKind::Turret, Row->CreaftingImage, SlotNum);
		}

		break;
	case ECreaftingKind::Structures:
		RowNames = StructuresCraftingDataTable->GetRowNames();
		for (int SlotNum = 0; SlotNum < RowNames.Num(); SlotNum++) {
			FCreafting* Row = StructuresCraftingDataTable->FindRow<FCreafting>(RowNames[SlotNum], TEXT("General"));

			CreateCraftingSlot(RowNames[SlotNum], ECreaftingKind::Structures, Row->CreaftingImage, SlotNum);
		}
		break;
	default:
		break;
	}
}

/// <summary>
/// 크래프팅 슬롯을 만들고 CreaftingGridPanel에 붙여주는 함수
/// </summary>
/// <param name="RowName">데이터 테이블의 RowName</param>
/// <param name="CraftingKind">만드는 종류</param>
/// <param name="CreaftingImage">슬롯에 나타낼 이미지</param>
/// <param name="SlotNum">슬롯 번호</param>
void UDTS_CreaftingGridUserWidget::CreateCraftingSlot(FName RowName, ECreaftingKind CraftingKind, UTexture2D* CreaftingImage, int SlotNum)
{
	FSoftObjectPath WidgetPath(TEXT("/Game/Kimmh/UI/Creafting/WBP_CraftingSlot.WBP_CraftingSlot_C"));

	UClass* CraftingSSlotUserWidgetClass = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), nullptr, *WidgetPath.ToString()));

	//TSubclassOf<UDTS_CraftingSlotUserWidget> CraftingSSlotUserWidgetClass;
	//CraftingSSlotUserWidgetClass = LoadClass<UDTS_CraftingSlotUserWidget>(nullptr, TEXT("/Game/Kimmh/UI/Creafting/WBP_CraftingSlot.WBP_CraftingSlot_C"));


	if (CraftingSSlotUserWidgetClass) {

		UDTS_CraftingSlotUserWidget* CraftSlotUserWidget
			= CreateWidget<UDTS_CraftingSlotUserWidget>(GetWorld()->GetFirstPlayerController(), CraftingSSlotUserWidgetClass);

		if (CraftSlotUserWidget) {
			// 크래프팅 슬롯이 생성되면 크래프팅 슬롯에 필요한 정보를 넘겨준다
			CraftSlotUserWidget->RowName = RowName;
			CraftSlotUserWidget->CraftingKind = CraftingKind;
			CraftSlotUserWidget->SetCraftingImage(CreaftingImage);

			// 위젯의 버튼 클릭델리게이트를 바인딩함
			CraftSlotUserWidget->
				OnButtonClicked.AddDynamic(
					this,
					&UDTS_CreaftingGridUserWidget::CallCreaftingInfo
				); // UDTS_CraftingSlotUserWidget의 버튼 클릭 델리게이트를 바인딩함

			if (CreaftingGridPanel) {
				CreaftingGridPanel->AddChildToUniformGrid(
					CraftSlotUserWidget,
					SlotNum / 5,
					SlotNum % 5
				);	// 슬롯 번호로 그리드에 붙여준다
			}
			else {
				UE_LOG(LogTemp, Log, TEXT("그리드 패널이 없다고요? 왜?"));
				FString Message = FString::Printf(TEXT("그리드 패널이 없다고요? 왜?"));
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, Message);
			}
		}
		else {
			UE_LOG(LogTemp, Log, TEXT("크래프팅 슬롯 생성 불가"));
			FString Message = FString::Printf(TEXT("크래프팅 슬롯 생성 불가"));
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, Message);
		}
	}

	
}

/// <summary>
/// UDTS_CraftingSlotUserWidget에서 보낸 델리게이트를 바인딩할 함수
/// </summary>
/// <param name="CreaftingKind">크래프팅 종류</param>
/// <param name="RowName">데이터 테이블의 이름</param>
void UDTS_CreaftingGridUserWidget::CallCreaftingInfo(ECreaftingKind CreaftingKind, FName RowName)
{

	UKismetSystemLibrary::PrintString(this, TEXT("CraftGrid Bind CraftSlot Delegate And Call"));

	OnGridPanelDelegate.Broadcast(CreaftingKind, RowName);	// 크래프팅 아이템 정보 위젯을 갱신하라는 델리게이트 호출
}

