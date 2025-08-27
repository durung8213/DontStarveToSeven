// Fill out your copyright notice in the Description page of Project Settings.


#include "Kimmh/HUDnUI/HUD/DTS_BaseGamePlayHUD.h"
#include "Kimmh/HUDnUI/UI/DTS_PlayerMainUserWidget.h"
#include "Kimmh/HUDnUI/UI/DTS_CreaftingUserWidget.h"
#include "Kimmh/HUDnUI/UI/DTS_UserStateUserWidget.h"
#include "Hamkt/Enemy/EnemyBoss/EnemyBossBase.h"
#include "Engine/AssetManager.h"

ADTS_BaseGamePlayHUD::ADTS_BaseGamePlayHUD()
{
	//static ConstructorHelpers::FClassFinder<UDTS_CreaftingUserWidget> CraftingUserWidgetPath(TEXT("/Game/Kimmh/UI/Creafting/WBP_Creafting.WBP_Creafting_C"));
	//if (CraftingUserWidgetPath.Succeeded()) {
	//	DTS_CreaftingUserWidgetClass = CraftingUserWidgetPath.Class;
	//}
	//'/Game/Kimmh/UI/Creafting/WBP_Creafting.WBP_Creafting'
}

void ADTS_BaseGamePlayHUD::BeginPlay()
{
	Super::BeginPlay();


	FSoftObjectPath UserStateWidgetPath(TEXT("/Game/Kimmh/UI/PlayerStateUI/WBP_UserState.WBP_UserState_C"));

	UClass* UserStateWidgetClass = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), nullptr, *UserStateWidgetPath.ToString()));

	if (UserStateWidgetClass) {
		DTSUserStateUserWidget = CreateWidget<UDTS_UserStateUserWidget>(GetWorld()->GetFirstPlayerController(), UserStateWidgetClass);
		if (DTSUserStateUserWidget) {
			DTSUserStateUserWidget->AddToViewport();
			DTSUserStateUserWidget->Init();
		}
		else {
			UE_LOG(LogTemp, Log, TEXT("DTSPlayerMainUserWidget 생성 불가"));
		}

	}
	else {
		UE_LOG(LogTemp, Log, TEXT("PlayerMainWidgetClass 변환 불가"));
	}
}

void ADTS_BaseGamePlayHUD::OpenPlayerMain()
{
	if (DTSPlayerMainUserWidget) 
	{
		DTSPlayerMainUserWidget->AddToViewport();
	}
	else 
	{
		CreatePlayerMainWidget();
	}

}

void ADTS_BaseGamePlayHUD::ClosePlayerMain()
{
	if (DTSPlayerMainUserWidget) 
	{
		DTSPlayerMainUserWidget->RemoveFromParent();
		DTSPlayerMainUserWidget = nullptr;
	}
}

void ADTS_BaseGamePlayHUD::CreatePlayerMainWidget()
{
	FSoftObjectPath PlayerMainWidgetPath(TEXT("/Game/Kimmh/UI/Inventory/WBP_PlayerMain.WBP_PlayerMain_C"));

	UClass* PlayerMainWidgetClass = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), nullptr, *PlayerMainWidgetPath.ToString()));

	if (PlayerMainWidgetClass) {
		DTSPlayerMainUserWidget = CreateWidget<UDTS_PlayerMainUserWidget>(GetWorld()->GetFirstPlayerController(), PlayerMainWidgetClass);
		if (DTSPlayerMainUserWidget) {
			DTSPlayerMainUserWidget->AddToViewport();
		}
		else {
			UE_LOG(LogTemp, Log, TEXT("DTSPlayerMainUserWidget 생성 불가"));
		}

	}
	else {
		UE_LOG(LogTemp, Log, TEXT("PlayerMainWidgetClass 변환 불가"));
	}

}

void ADTS_BaseGamePlayHUD::ReLoadInventory()
{
	if (DTSPlayerMainUserWidget) {
		DTSPlayerMainUserWidget->ReLoadInventory();
	}
	if (DTSCreaftingUserWidget) {
		DTSCreaftingUserWidget->ReloadRequireItemCount();
	}
}

void ADTS_BaseGamePlayHUD::ReLoadQuickSlot()
{
	if (DTSPlayerMainUserWidget) {
		DTSPlayerMainUserWidget->ReloadQuickSlot();
	}

	if (DTSUserStateUserWidget) {
		DTSUserStateUserWidget->ReLoadQuickSlot();
	}
}

void ADTS_BaseGamePlayHUD::OpenCreaftingUI()
{
	if (DTSCreaftingUserWidget)
	{
		DTSCreaftingUserWidget->AddToViewport();
	}
	else
	{
		CreateCreaftingUI();
	}
}

void ADTS_BaseGamePlayHUD::CloseCreaftingUI()
{
	if (DTSCreaftingUserWidget)
	{
		DTSCreaftingUserWidget->RemoveFromParent();
		DTSCreaftingUserWidget = nullptr;
	}
}

void ADTS_BaseGamePlayHUD::CreateCreaftingUI()
{
	FSoftObjectPath CreaftingWidgetPath(TEXT("/Game/Kimmh/UI/Creafting/WBP_Creafting.WBP_Creafting_C"));

	UClass* CreaftingWidgetClass = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), nullptr, *CreaftingWidgetPath.ToString()));

	if (CreaftingWidgetClass) {
		DTSCreaftingUserWidget = CreateWidget<UDTS_CreaftingUserWidget>(GetWorld()->GetFirstPlayerController(), DTS_CreaftingUserWidgetClass);
		if (DTSCreaftingUserWidget) {
			DTSCreaftingUserWidget->AddToViewport();
		}
		else {
			UE_LOG(LogTemp, Log, TEXT("DTSPlayerMainUserWidget 생성 불가"));
		}

	}
	else {
		UE_LOG(LogTemp, Log, TEXT("PlayerMainWidgetClass 변환 불가"));
	}
}

void ADTS_BaseGamePlayHUD::SelectQuickSlot(int SlotNum)
{
	DTSUserStateUserWidget->SelectQuickSlot(SlotNum);
}

void ADTS_BaseGamePlayHUD::BindBossTakeDamage(AEnemyBossBase* Boss)
{
	if(Boss) {
		Boss->OnTakeDamage.AddUniqueDynamic(this, &ADTS_BaseGamePlayHUD::SetBossHpBarPercent);
	}
}

void ADTS_BaseGamePlayHUD::UnBindBossTakeDamage(AEnemyBossBase* Boss)
{
	if (Boss) {
		Boss->OnTakeDamage.RemoveDynamic(this, &ADTS_BaseGamePlayHUD::SetBossHpBarPercent);
	}
}

void ADTS_BaseGamePlayHUD::SetBossHpBarVisibility(bool bIsVisbile)
{
	DTSUserStateUserWidget->SetBossHpBarVisibility(bIsVisbile);
}

void ADTS_BaseGamePlayHUD::SetBossHpBarPercent(float InCurrentHealth, float InMaxHealth)
{
	DTSUserStateUserWidget->SetBossHpBarPercent(InCurrentHealth, InMaxHealth);
}

void ADTS_BaseGamePlayHUD::SetBossName(FText BossName)
{
	DTSUserStateUserWidget->SetBossName(BossName);
}




