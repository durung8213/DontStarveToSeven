// Fill out your copyright notice in the Description page of Project Settings.


#include "Kimmh/HUDnUI/UI/DTS_ExitGameUpUserWidget.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"

void UDTS_ExitGameUpUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	NoButton->OnClicked.AddDynamic(this, &UDTS_ExitGameUpUserWidget::OnNoButtonClick);
}

void UDTS_ExitGameUpUserWidget::OnOkButtonClick()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, true);
}

void UDTS_ExitGameUpUserWidget::OnNoButtonClick()
{
	SetVisibility(ESlateVisibility::Collapsed);
}
