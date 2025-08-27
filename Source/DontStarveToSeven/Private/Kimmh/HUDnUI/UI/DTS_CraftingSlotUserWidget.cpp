// Fill out your copyright notice in the Description page of Project Settings.


#include "Kimmh/HUDnUI/UI/DTS_CraftingSlotUserWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Kimmh/Enum/CreaftingKindEnum.h"
#include "Kimmh/HUDnUI/HUD/DTS_BaseGamePlayHUD.h"

/// <summary>
/// UI�� �ҷ������� ��ư ���ε�
/// </summary>
void UDTS_CraftingSlotUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	// ��ư�� Ŭ�� �Ǿ��� �� �Լ� ����
	CreaftingSlotButton->OnClicked.AddDynamic(this, &UDTS_CraftingSlotUserWidget::CallCraftingInfo);
}

/// <summary>
/// ���� ��� ���Կ� ���� �̹��� ����
/// </summary>
/// <param name="Image">���� �̹���</param>
void UDTS_CraftingSlotUserWidget::SetCraftingImage(UTexture2D* Image)
{
	// UImage�� �̹��� ����
	CraftingImage->SetBrushFromTexture(Image);
}

/// <summary>
/// CraftItemInfo�� ��� ���� ���� ��������Ʈ�� ȣ���ϴ� �Լ�
/// </summary>
void UDTS_CraftingSlotUserWidget::CallCraftingInfo()
{
	// ������ ����̶� �ǴܵǾ� ����
	//ADTS_BaseGamePlayHUD* BaseGamePlayHUD = Cast<ADTS_BaseGamePlayHUD>(GetOwningPlayer()->GetHUD());
	
	// ��������Ʈ ȣ��
	OnButtonClicked.Broadcast(CraftingKind, RowName);

	UE_LOG(LogTemp, Log, TEXT("Creafting Slot Button Click"));
	FString Message = FString::Printf(TEXT("Creafting Slot Button Click"));
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, Message);
}
