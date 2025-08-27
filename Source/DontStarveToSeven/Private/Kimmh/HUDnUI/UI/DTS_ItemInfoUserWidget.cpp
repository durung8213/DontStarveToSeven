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
/// ������ ����â�� �������� �����ϴ� �Լ�
/// </summary>
/// <param name="RowName">������ RowName</param>
void UDTS_ItemInfoUserWidget::SetItem(FString RowName)
{
	// 1. ��Ÿ�� ���� ������ ���̺� ��������
	UDataTable* DataTable;		// ������ ���̺� ���� ����
	DataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Leesh/Table/DT_TestItemInfo.DT_TestItemInfo"));	// ������ ������ ���̺� �ҷ���
	if (DataTable) {
		// ������ ���̺��� �ҷ����µ� �����ϸ�
		FTestItemInfo* Row = DataTable->FindRow<FTestItemInfo>(FName(*RowName), TEXT("General"));
		if (Row) {
			// ã������ �������� ������
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
/// ������ ����â�� �������� ����� �����ϴ� �Լ� (����â �� �κ� ���� ����)
/// </summary>
/// <param name="ItemGrade">������ ���</param>
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
/// ������ ���� â�� ������ �̸��� �����ϴ� �Լ�
/// </summary>
/// <param name="ItemName">������ �̸�</param>
void UDTS_ItemInfoUserWidget::SetItemName(FText ItemName)
{
	ItemNameTextBlock->SetText(ItemName);
}

/// <summary>
/// ������ ���� â�� ������ Ÿ���� �����ϴ� �Լ�
/// </summary>
/// <param name="ItemTpye">������ Ÿ��</param>
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
/// ������ ���� â�� ������ ������ �����ϴ� �Լ�
/// </summary>
/// <param name="ItemDescription">������ ����</param>
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
/// ������ ���� â�� �Լ� ��θ� �����ϴ� �Լ�
/// </summary>
/// <param name="ItemDropPath">������ ����</param>
void UDTS_ItemInfoUserWidget::SetItemDropPath(FText ItemDropPath)
{
	ItemDropPathTextBlock->SetText(ItemDropPath);
}


