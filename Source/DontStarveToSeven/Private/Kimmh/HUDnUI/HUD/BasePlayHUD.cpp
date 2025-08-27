// Fill out your copyright notice in the Description page of Project Settings.


#include "Kimmh/HUDnUI/HUD/BasePlayHUD.h"
#include "Kimmh/HUDnUI/UI/DTS_PlayerMainUserWidget.h"
#include "Engine/AssetManager.h"
#include "Kismet/KismetSystemLibrary.h"


void ABasePlayHUD::BeginPlay()
{
	Super::BeginPlay();


	FSoftObjectPath PlayerMainWidgetPath(TEXT("/Game/Kimmh/UI/Inventory/WBP_PlayerMain.WBP_PlayerMain_C"));

	UClass* PlayerMainWidgetClass = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), nullptr, *PlayerMainWidgetPath.ToString()));

	if (PlayerMainWidgetClass)
	{
		DTSPlayerMainUserWidget = CreateWidget<UDTS_PlayerMainUserWidget>(GetWorld()->GetFirstPlayerController(), PlayerMainWidgetClass);
		if (DTSPlayerMainUserWidget)
		{
			DTSPlayerMainUserWidget->AddToViewport();
		}
		else
		{
			UKismetSystemLibrary::PrintString(this, TEXT("DTSPlayerMainUserWidget Create Fail"));
		}
	}
	else
	{
		UKismetSystemLibrary::PrintString(this, TEXT("PlayerMainWidgetClass Load Fail"));
	}
}
