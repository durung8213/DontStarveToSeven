// Fill out your copyright notice in the Description page of Project Settings.


#include "Kimmh/HUDnUI/UI/DTS_CraftingInfoUserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UDTS_CraftingInfoUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

/// <summary>
/// Info������ �ִ� ������ ������(�ʱ�ȭ�� �����)
/// </summary>
void UDTS_CraftingInfoUserWidget::ClearInfo()
{
	CraftItemImage->SetBrushFromTexture(nullptr);
	CraftItemName->SetText(FText::FromString(TEXT("")));
	CraftItemRequireStructure->SetText(FText::FromString(TEXT("")));
	CraftItemExplain->SetText(FText::FromString(TEXT("")));
}

/// <summary>
/// ������ �������� ��ҵ��� �Ű������� �޾ƿ� ǥ��
/// </summary>
/// <param name="ItemImage">������ ������ �̹���</param>
/// <param name="ItemName">������ ������ �̸�</param>
/// <param name="ItemRequireStructure">������ ���ۿ� �ʿ��� ������</param>
/// <param name="ItemExplain">������ ������ ����</param>
void UDTS_CraftingInfoUserWidget::SetItemInfo(UTexture2D* ItemImage, FText ItemName, FText ItemRequireStructure, FText ItemExplain)
{
	CraftItemImage->SetBrushFromTexture(ItemImage);
	CraftItemName->SetText(ItemName);
	CraftItemRequireStructure->SetText(ItemRequireStructure);
	CraftItemExplain->SetText(ItemExplain);
}
