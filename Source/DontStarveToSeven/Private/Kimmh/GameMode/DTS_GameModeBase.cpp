// Fill out your copyright notice in the Description page of Project Settings.


#include "Kimmh/GameMode/DTS_GameModeBase.h"
#include "Kimmh/HUDnUI/UI/DTS_TitleUserWidget.h"
#include "Kimmh/HUDnUI/UI/DTS_PlayerMainUserWidget.h"
#include "Kimmh/HUDnUI/UI/DTS_UserStateUserWidget.h"

ADTS_GameModeBase::ADTS_GameModeBase()
{
	static ConstructorHelpers::FClassFinder<UDTS_TitleUserWidget> Dummy(TEXT("/Game/Kimmh/UI/TItle/WBP_Title.WBP_Title_C"));
	static ConstructorHelpers::FClassFinder<UDTS_PlayerMainUserWidget> Dummy2(TEXT("/Game/Kimmh/UI/Inventory/WBP_PlayerMain.WBP_PlayerMain_C"));
	static ConstructorHelpers::FClassFinder<UDTS_UserStateUserWidget> Dummy3(TEXT("/Game/Kimmh/UI/PlayerStateUI/WBP_UserState.WBP_UserState_C"));

}
