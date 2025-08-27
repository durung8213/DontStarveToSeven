// Fill out your copyright notice in the Description page of Project Settings.


#include "Kimmh/HUDnUI/UI/DTS_CraftingSlotUserWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Kimmh/Enum/CreaftingKindEnum.h"
#include "Kimmh/HUDnUI/HUD/DTS_BaseGamePlayHUD.h"

/// <summary>
/// UI가 불러와지면 버튼 바인드
/// </summary>
void UDTS_CraftingSlotUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	// 버튼이 클릭 되었을 때 함수 실행
	CreaftingSlotButton->OnClicked.AddDynamic(this, &UDTS_CraftingSlotUserWidget::CallCraftingInfo);
}

/// <summary>
/// 제작 목록 슬롯에 있을 이미지 설정
/// </summary>
/// <param name="Image">슬롯 이미지</param>
void UDTS_CraftingSlotUserWidget::SetCraftingImage(UTexture2D* Image)
{
	// UImage에 이미지 넣음
	CraftingImage->SetBrushFromTexture(Image);
}

/// <summary>
/// CraftItemInfo에 띄울 정보 전달 델리게이트를 호출하는 함수
/// </summary>
void UDTS_CraftingSlotUserWidget::CallCraftingInfo()
{
	// 위험한 방식이라 판단되어 보류
	//ADTS_BaseGamePlayHUD* BaseGamePlayHUD = Cast<ADTS_BaseGamePlayHUD>(GetOwningPlayer()->GetHUD());
	
	// 델리게이트 호출
	OnButtonClicked.Broadcast(CraftingKind, RowName);

	UE_LOG(LogTemp, Log, TEXT("Creafting Slot Button Click"));
	FString Message = FString::Printf(TEXT("Creafting Slot Button Click"));
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, Message);
}
