// Fill out your copyright notice in the Description page of Project Settings.


#include "Kimmh/HUDnUI/HUD/DTS_BaseTitleHUD.h"
#include "Kimmh/HUDnUI/UI/DTS_TitleUserWidget.h"
#include "Engine/AssetManager.h"
#include "Kismet/KismetSystemLibrary.h"

ADTS_BaseTitleHUD::ADTS_BaseTitleHUD()
{

}

void ADTS_BaseTitleHUD::BeginPlay()
{
	Super::BeginPlay();

	FSoftObjectPath WidgetPath(TEXT("/Game/Kimmh/UI/TItle/WBP_Title.WBP_Title_C"));

	UClass* WidgetClass = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), nullptr, *WidgetPath.ToString()));

	if (WidgetClass)
	{
		TitleUserWidget = CreateWidget<UDTS_TitleUserWidget>(GetWorld()->GetFirstPlayerController(), WidgetClass);
		if (TitleUserWidget)
		{
			TitleUserWidget->AddToViewport();
		}
		else
		{
			UKismetSystemLibrary::PrintString(this, TEXT("DTS_TitleUserWidget Create Fail"));
		}
	}
	else
	{
		UKismetSystemLibrary::PrintString(this, TEXT("WidgetClass Load Fail"));
	}

}
