// Fill out your copyright notice in the Description page of Project Settings.


#include "Kimmh/HUDnUI/UI/DTS_BasePopUpUserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UDTS_BasePopUpUserWidget::NativeConstruct()
{
	EmptySpaceButton->OnClicked.AddDynamic(this, &UDTS_BasePopUpUserWidget::OnEmptySpaceButtonClick);
	OkButton->OnClicked.AddDynamic(this, &UDTS_BasePopUpUserWidget::OnOkButtonClick);
}

void UDTS_BasePopUpUserWidget::OnEmptySpaceButtonClick()
{
	// ÆË¾÷ Ã¢ ¹İÂ¦ ¹İÂ¦!
}

void UDTS_BasePopUpUserWidget::OnOkButtonClick()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UDTS_BasePopUpUserWidget::SetPopUpMessage(const FText& Message)
{
	MessageTextBlock->SetText(Message);
}



