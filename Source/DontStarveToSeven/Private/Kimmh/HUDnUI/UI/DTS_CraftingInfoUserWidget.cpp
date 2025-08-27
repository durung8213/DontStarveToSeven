// Fill out your copyright notice in the Description page of Project Settings.


#include "Kimmh/HUDnUI/UI/DTS_CraftingInfoUserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UDTS_CraftingInfoUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

/// <summary>
/// Info위젯에 있는 모든것을 지워줌(초기화에 가까움)
/// </summary>
void UDTS_CraftingInfoUserWidget::ClearInfo()
{
	CraftItemImage->SetBrushFromTexture(nullptr);
	CraftItemName->SetText(FText::FromString(TEXT("")));
	CraftItemRequireStructure->SetText(FText::FromString(TEXT("")));
	CraftItemExplain->SetText(FText::FromString(TEXT("")));
}

/// <summary>
/// 제작할 아이템의 요소들을 매개변수로 받아와 표시
/// </summary>
/// <param name="ItemImage">제작할 아이템 이미지</param>
/// <param name="ItemName">제작할 아이템 이름</param>
/// <param name="ItemRequireStructure">아이템 제작에 필요한 구조물</param>
/// <param name="ItemExplain">제작할 아이템 설명</param>
void UDTS_CraftingInfoUserWidget::SetItemInfo(UTexture2D* ItemImage, FText ItemName, FText ItemRequireStructure, FText ItemExplain)
{
	CraftItemImage->SetBrushFromTexture(ItemImage);
	CraftItemName->SetText(ItemName);
	CraftItemRequireStructure->SetText(ItemRequireStructure);
	CraftItemExplain->SetText(ItemExplain);
}
