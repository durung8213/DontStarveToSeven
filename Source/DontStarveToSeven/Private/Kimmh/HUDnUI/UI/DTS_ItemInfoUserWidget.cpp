// Fill out your copyright notice in the Description page of Project Settings.


#include "Kimmh/HUDnUI/UI/DTS_ItemInfoUserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"


void UDTS_ItemInfoUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UDTS_ItemInfoUserWidget::OnVisible()
{
	this->SetVisibility(ESlateVisibility::Visible);
}

void UDTS_ItemInfoUserWidget::OnCollapsed()
{
	this->SetVisibility(ESlateVisibility::Collapsed);
}

/// <summary>
/// 아이템 정보창에 아이템을 세팅하는 함수
/// </summary>
/// <param name="RowName">아이템 RowName</param>
void UDTS_ItemInfoUserWidget::SetItem(FString RowName)
{
	// 1. 런타임 도중 데이터 테이블 가져오기
	UDataTable* DataTable;		// 데이터 테이블 변수 선언
	DataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Leesh/Table/DT_TestItemInfo.DT_TestItemInfo"));	// 아이템 데이터 테이블 불러옴
	if (DataTable) {
		// 데이터 테이블을 불러오는데 성공하면
		FTestItemInfo* Row = DataTable->FindRow<FTestItemInfo>(FName(*RowName), TEXT("General"));
		if (Row) {
			// 찾을려는 아이템이 있으면
			SetItemGrade(Row->ItemGrade);
			SetItemName(Row->ItemName);
			SetItemType(Row->ItemType);
			SetItemExplain(Row->ItemDescription, Row->ItemStatChanges);
			SetItemDropPath(Row->ItemDropPath);
		}
	}
	else {

	}
}

/// <summary>
/// 아이템 정보창에 아이템의 등급을 세팅하는 함수 (정보창 윗 부분 색이 변경)
/// </summary>
/// <param name="ItemGrade">아이템 등급</param>
void UDTS_ItemInfoUserWidget::SetItemGrade(EItemGrade ItemGrade)
{
	FLinearColor GradeColor;
	switch (ItemGrade)
	{
	case EItemGrade::Common:
		GradeColor = FLinearColor(0.8f, 0.8f, 0.8f, 1.0f);
		break;
	case EItemGrade::Uncommon:
		GradeColor = FLinearColor(0.0f, 0.8f, 0.0f, 1.0f);
		break;
	case EItemGrade::Rare:
		GradeColor = FLinearColor(0.0f, 0.5f, 1.0f, 1.0f);
		break;
	case EItemGrade::Epic:
		GradeColor = FLinearColor(0.45995f, 0.099275f, 1.f, 0.836f);
		break;
	case EItemGrade::Legendary:
		GradeColor = FLinearColor(1.0f, 0.6f, 0.0f, 1.0f);
		break;
	default:
		GradeColor = FLinearColor(0.8f, 0.8f, 0.8f, 1.0f);
		break;
	}

	ItemGradeBackground->SetColorAndOpacity(GradeColor);
}

/// <summary>
/// 아이템 정보 창에 아이템 이름을 설정하는 함수
/// </summary>
/// <param name="ItemName">아이템 이름</param>
void UDTS_ItemInfoUserWidget::SetItemName(FText ItemName)
{
	ItemNameTextBlock->SetText(ItemName);
}

/// <summary>
/// 아이템 정보 창에 아이템 타입을 설정하는 함수
/// </summary>
/// <param name="ItemTpye">아이템 타입</param>
void UDTS_ItemInfoUserWidget::SetItemType(ETestItemType ItemTpye)
{
	FString StringValue;// = TEXT("");
	
	switch (ItemTpye)
	{
	case ETestItemType::None:
		StringValue = TEXT("None");
		break;
	case ETestItemType::Weapon:
		StringValue = TEXT("Weapon");
		break;
	case ETestItemType::Armor:
		StringValue = TEXT("Armor");
		break;
	case ETestItemType::CraftMaterial:
		StringValue = TEXT("Resource");
		break;
	case ETestItemType::Consume:
		StringValue = TEXT("Consume");
		break;
	case ETestItemType::Etc:
		StringValue = TEXT("Etc");
		break;
	default:
		StringValue = TEXT("ETestItemType is not specified");
		break;
	}
	ItemTypeTextBlock->SetText(FText::FromString(StringValue));
}

/// <summary>
/// 아이템 정보 창에 아이템 설명을 설정하는 함수
/// </summary>
/// <param name="ItemDescription">아이템 설명</param>
void UDTS_ItemInfoUserWidget::SetItemExplain(FText ItemDescription, FTestItemStatChanges ItemStatChanges)
{
	bool StatChange = false;
	FString StringValue = TEXT("");
	if (ItemStatChanges.MaxHP != 0) {
		StringValue.Appendf(TEXT("MaxHP + %d\n"), ItemStatChanges.MaxHP);
		StatChange = true;
	}

	if (ItemStatChanges.MaxMP != 0) {
		StringValue.Appendf(TEXT("MaxMP + %d\n"), ItemStatChanges.MaxMP);
		StatChange = true;
	}

	if (ItemStatChanges.Atk != 0) {
		StringValue.Appendf(TEXT("Atk + %d\n"), ItemStatChanges.Atk);
		StatChange = true;
	}

	if (ItemStatChanges.Def != 0) {
		StringValue.Appendf(TEXT("Def + %d\n"), ItemStatChanges.Def);
		StatChange = true;
	}

	if (ItemStatChanges.Critical != 0) {
		StringValue.Appendf(TEXT("Critical + %d\n"), ItemStatChanges.Critical);
		StatChange = true;
	}

	if (ItemStatChanges.CriticalDamage != 0) {
		StringValue.Appendf(TEXT("CriticalDamage + %d\n"), ItemStatChanges.CriticalDamage);
		StatChange = true;
	}

	if (ItemStatChanges.Speed != 0) {
		StringValue.Appendf(TEXT("Speed + %d\n"), ItemStatChanges.Speed);
		StatChange = true;
	}

	if (StatChange) {
		StringValue.Appendf(TEXT("\n\n"));
		ItemExplainTextBlock->SetText(FText::Join(FText::FromString(TEXT("")), FText::FromString(StringValue), ItemDescription));
	}
	else {
		ItemExplainTextBlock->SetText(ItemDescription);
	}

	ItemExplainTextBlock->SetText(FText::Join(FText::FromString(TEXT("")), FText::FromString(StringValue), ItemDescription));
}

/// <summary>
/// 아이템 정보 창에 입수 경로를 설정하는 함수
/// </summary>
/// <param name="ItemDropPath">아이템 설명</param>
void UDTS_ItemInfoUserWidget::SetItemDropPath(FText ItemDropPath)
{
	ItemDropPathTextBlock->SetText(ItemDropPath);
}


