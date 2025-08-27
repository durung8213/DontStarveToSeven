// Fill out your copyright notice in the Description page of Project Settings.


#include "Kimmh/HUDnUI/UI/DTS_CraftItemCreateUserWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Kimmh/Character/Controller/DTS_GamePlayPlayerController.h"
#include "Kimmh/HUDnUI/UI/DTS_RequireItemSlotUserWidget.h"
#include "Kimmh/BasePlayerCharacter.h"
#include "Seojy/CraftManager.h"
#include "Kismet/KismetSystemLibrary.h"


void UDTS_CraftItemCreateUserWidget::NativeConstruct()
{
    Super::NativeConstruct();
	MakeCountTextBox->OnTextChanged.AddDynamic(
        this, 
        &UDTS_CraftItemCreateUserWidget::OnMakeCountTextBoxChanged
    );  // ũ������ ������ ��Ÿ�� �����ͺ� �ý�Ʈ �ڽ��� ������ ������ �� �����ϴ� �Լ� ���ε�

    SubButton->OnClicked.AddDynamic(
        this, 
        &UDTS_CraftItemCreateUserWidget::OnSubButtonClick
    );  // Ŭ�� �Լ� ���ε�

    PlusButton->OnClicked.AddDynamic(
        this, 
        &UDTS_CraftItemCreateUserWidget::OnPlusButtonClick
    );  // Ŭ�� �Լ� ���ε�

    MaxButton->OnClicked.AddDynamic(
        this, 
        &UDTS_CraftItemCreateUserWidget::OnMaxButtonClick
    );  // Ŭ�� �Լ� ���ε�

    CraftButton->OnClicked.AddDynamic(
        this, 
        &UDTS_CraftItemCreateUserWidget::OnCraftButtonClick
    );  // Ŭ�� �Լ� ���ε�
}

/// <summary>
/// MakeCountTextBox�� ������ �ٲ�� �ش� ���� �������� Ȯ���ϰ� 
/// ���ڸ� ���� ���ڰ� �ƴϸ� �������� �ʰ� �ϴ� �Լ�
/// </summary>
/// <param name="Text">�ٲ� ����</param>
void UDTS_CraftItemCreateUserWidget::OnMakeCountTextBoxChanged(const FText& Text)
{
    FString FilteredText;
    for (TCHAR Character : Text.ToString())
    {
        if (FChar::IsDigit(Character))  // ������ ��쿡�� �߰�
        {
            // ���ڰ� ������ ���� �ý�Ʈ�� ����
            FilteredText.AppendChar(Character);
        }
    }

    // �ý�Ʈ ����
    if (FilteredText != Text.ToString())
    {
        MakeCountTextBox->SetText(FText::FromString(FilteredText));
    }

}


/// <summary>
/// MakeCountTextBox�� ���ڸ� 1 ���� �Լ�
/// </summary>
void UDTS_CraftItemCreateUserWidget::OnSubButtonClick()
{
    FString TextString = MakeCountTextBox->GetText().ToString();
    if (TextString.IsNumeric()) {
        // �� �κ��� �ٷ� �� �Լ��� �ִ°Ŷ� 73��° �� ���� ������ ������ �װ��� �� ��
        int32 Count = FCString::Atoi(*MakeCountTextBox->GetText().ToString());
        if (Count > 1) {
            --Count;
            MakeCountTextBox->SetText(FText::AsNumber(Count));
            for (UDTS_RequireItemSlotUserWidget* RequireItemSlot : CurrentCraftItemRequireSlotArr) {
                RequireItemSlot->SetItemCountTotal(RequireItemSlot->RequireItemCount * Count);
            }
        }
    }
    else {
        UKismetSystemLibrary::PrintString(this, TEXT("���� ī��Ʈ �ý�Ʈ ������ ������ ���ڷ� ���� �Ұ�"));
    }

}

/// <summary>
/// MakeCountTextBox�� ���ڸ� 1 ���ϴ� �Լ�
/// </summary>
void UDTS_CraftItemCreateUserWidget::OnPlusButtonClick()
{
    FString TextString = MakeCountTextBox->GetText().ToString();
    if (TextString.IsNumeric()) {
        // �����ͺ� �ý�Ʈ �ڽ��� �ִ� ���� ���ڸ�
        int32 Count = FCString::Atoi(*MakeCountTextBox->GetText().ToString());  // int32�� ����ȯ
        Count++;    // ������Ŵ
       
        MakeCountTextBox->SetText(FText::AsNumber(Count));  // �����ͺ� �ý�Ʈ�� ���� ����
        for (UDTS_RequireItemSlotUserWidget* RequireItemSlot : CurrentCraftItemRequireSlotArr) {
            RequireItemSlot
                ->SetItemCountTotal
                (
                    RequireItemSlot->RequireItemCount * Count
                );  // �ʿ� ���� * ���� ������ ���� ����
        }
    }
    else {
        UE_LOG(LogTemp, Log, TEXT("���� ī��Ʈ �ý�Ʈ ������ ������ ���ڷ� ���� �Ұ�"));
        FString Message = FString::Printf(TEXT("���� ī��Ʈ �ý�Ʈ ������ ������ ���ڷ� ���� �Ұ�"));
        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, Message);
    }
}

/// <summary>
/// MakeCountTextBox�� ���ڸ� ���� �� �ִ� �ִ� ������ �ø��� �Լ�
/// </summary>
void UDTS_CraftItemCreateUserWidget::OnMaxButtonClick()
{
    // �� �κ��丮 ����̶� �����ؾ� ��
    UE_LOG(LogTemp, Log, TEXT("Inventory Marge Require"));
    FString Message = FString::Printf(TEXT("Inventory Marge Require"));
    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, Message);
}

/// <summary>
/// ���� ��ư
/// </summary>
void UDTS_CraftItemCreateUserWidget::OnCraftButtonClick()
{
    ADTS_GamePlayPlayerController* CurrentPlayerController;
    ABasePlayerCharacter* CurrentPlayer;
    FString Message;
    bool bIsCanCreateItem = true;
    switch (CraftType)
    {
    case ECreaftingType::Item:
        // �������̸� (�κ��丮 ���� ����..�ϴ°� �ʿ�)
        CurrentPlayerController = Cast<ADTS_GamePlayPlayerController>(GetOwningPlayer());
        if (CurrentPlayerController) {
            for (UDTS_RequireItemSlotUserWidget* RequireSlot : CurrentCraftItemRequireSlotArr) {
                if (RequireSlot->TotalRequireItemCount > CurrentPlayerController->SearchItemCountToInventory(RequireSlot->ResourceItemName)) {
                    bIsCanCreateItem = false;
                }
            }
            if (bIsCanCreateItem) {
                for (UDTS_RequireItemSlotUserWidget* RequireSlot : CurrentCraftItemRequireSlotArr) {
                    CurrentPlayerController->RemoveItemToInventoryByName(RequireSlot->ResourceItemName, RequireSlot->TotalRequireItemCount);
                }
                
                CurrentPlayerController->AddItemToInventoryByName(CraftItemDBRowName.ToString(), FCString::Atoi(*MakeCountTextBox->GetText().ToString()));
            }
        }
        break;
    case ECreaftingType::Build:
        // ���๰�̸�
        CurrentPlayerController = Cast<ADTS_GamePlayPlayerController>(GetOwningPlayer());
        if (CurrentPlayerController) {
            // �÷��̾� ��Ʈ�ѷ��� �������� IMC�� ���۸��� ����
            CurrentPlayerController->ChangeInputMappingContext(EIMC::CreaftingIMC);
            CurrentPlayer = Cast<ABasePlayerCharacter>(CurrentPlayerController->GetPawn());
            if (CurrentPlayer) {
                // ũ������ �޴����� ~�� ���� ���̶�� ����
                // �ֿ� ����
                CurrentPlayer->CraaftManager->StartCraftMode(CraftStructureType);
            }
        }

        UE_LOG(LogTemp, Log, TEXT("CraftMode Active"));
        Message = FString::Printf(TEXT("CraftMode Active"));
        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, Message);
        break;
    default:
        break;
    }
}

/// <summary>
/// �ʿ��� ������ ���� ������ �߰��� ���� �ڽ��� ���� �Լ�
/// </summary>
void UDTS_CraftItemCreateUserWidget::ClearNeedItemSlotVerticalBox()
{
    if(NeedItemSlotVerticalBox)
        // ��Ƽ�� �ڽ� �� �ڽĵ��� ����
        NeedItemSlotVerticalBox->ClearChildren();

    if (!CurrentCraftItemRequireSlotArr.IsEmpty()) {
        // ���� �迭 ���
        CurrentCraftItemRequireSlotArr.Empty();
    }
}

/// <summary>
/// �� UI�� �Ⱥ��̰� �ϴ� �Լ�
/// </summary>
void UDTS_CraftItemCreateUserWidget::CollispedCreafting()
{
    // �ִ� ��� ���� �Ⱥ��̰� ����
    SubButton->SetVisibility(ESlateVisibility::Collapsed);
    PlusButton->SetVisibility(ESlateVisibility::Collapsed);
    MaxButton->SetVisibility(ESlateVisibility::Collapsed);
    CraftButton->SetVisibility(ESlateVisibility::Collapsed);
    MakeCountTextBox->SetText(FText::AsNumber(1));
    MakeCountTextBox->SetVisibility(ESlateVisibility::Collapsed);

    // ��� ������ ���� �ڽ��� ���� �Լ� ����
    ClearNeedItemSlotVerticalBox();
}

/// <summary>
/// �� UI�� ���̰� �ϴ� �Լ�
/// </summary>
void UDTS_CraftItemCreateUserWidget::VisibleCreafting()
{
    // �ִ� ��� ���� ���̰� ����
    SubButton->SetVisibility(ESlateVisibility::Visible);
    PlusButton->SetVisibility(ESlateVisibility::Visible);
    MaxButton->SetVisibility(ESlateVisibility::Visible);
    CraftButton->SetVisibility(ESlateVisibility::Visible);
    MakeCountTextBox->SetVisibility(ESlateVisibility::Visible);
}

void UDTS_CraftItemCreateUserWidget::SetCraftStructureType(ECraftType NewCreaftingType)
{
    CraftStructureType = NewCreaftingType;
}

void UDTS_CraftItemCreateUserWidget::ReloadRequireItemCount()
{
    ADTS_GamePlayPlayerController* CurrentPlayerController = Cast<ADTS_GamePlayPlayerController>(GetOwningPlayer());
    if (CurrentPlayerController) {
        for (UDTS_RequireItemSlotUserWidget* CraftItemRequireSlot : CurrentCraftItemRequireSlotArr) {

            if (CurrentPlayerController) {
                CraftItemRequireSlot->SetItemHave(
                    CurrentPlayerController->SearchItemCountToInventory(CraftItemRequireSlot->ResourceItemName)
                );
            }
            else {
                CraftItemRequireSlot->SetItemHave(0);    // �÷��̾ ������ �ִ� ������ ���� ����
            }
        }
    }
    
}

/// <summary>
/// ���ۿ� �ʿ��� ������ ������ ����� �Լ�
/// </summary>
/// <param name="ResourceItemName">�ʿ��� ������ �̸�</param>
/// <param name="ResoucreItemCount">�ʿ��� ������ ����</param>
void UDTS_CraftItemCreateUserWidget::CreateRequireItemSlot(FText ResourceItemName, int ResoucreItemCount)
{
    /*TSubclassOf<UDTS_RequireItemSlotUserWidget> CraftItemRequireSlotUserWidgetClass;

    CraftItemRequireSlotUserWidgetClass = LoadClass<UDTS_RequireItemSlotUserWidget>(nullptr, TEXT("/Game/Kimmh/UI/Creafting/WBP_RequireItemSlot.WBP_RequireItemSlot_C"));*/

    FSoftObjectPath WidgetPath(TEXT("/Game/Kimmh/UI/Creafting/WBP_RequireItemSlot.WBP_RequireItemSlot_C"));

    UClass* CraftItemRequireSlotUserWidgetClass = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), nullptr, *WidgetPath.ToString()));

    if (CraftItemRequireSlotUserWidgetClass) {
        UDTS_RequireItemSlotUserWidget* CraftItemRequireSlotCreateUserWidget = CreateWidget<UDTS_RequireItemSlotUserWidget>(GetWorld()->GetFirstPlayerController(), CraftItemRequireSlotUserWidgetClass);
        if (CraftItemRequireSlotCreateUserWidget) {
            // ���� ������ �����ϸ�

            NeedItemSlotVerticalBox->AddChildToVerticalBox(CraftItemRequireSlotCreateUserWidget);   // ������ ��Ƽ�� �ڽ� �ڽ����� ����
            CraftItemRequireSlotCreateUserWidget->SetItemAmount(ResoucreItemCount); // �ʿ� ������ ����(�ý�Ʈ�ڽ���, 1�� ����) ����
            CraftItemRequireSlotCreateUserWidget->RequireItemCount = ResoucreItemCount; // �ʿ� ������ ����(������, 1�� ����) ����
            CraftItemRequireSlotCreateUserWidget->SetItemName(ResourceItemName);   // �ʿ� ������ �̸� ����
            CraftItemRequireSlotCreateUserWidget->SetItemCountTotal(ResoucreItemCount * FCString::Atoi(*MakeCountTextBox->GetText().ToString())); // �ʿ� ������ ���� * ������ ������ ����

            ADTS_GamePlayPlayerController* CurrentPlayerController = Cast<ADTS_GamePlayPlayerController>(GetOwningPlayer());
            if (CurrentPlayerController) {
                CraftItemRequireSlotCreateUserWidget->SetItemHave(
                    CurrentPlayerController->SearchItemCountToInventory(ResourceItemName.ToString())
                );
            }
            else {
                CraftItemRequireSlotCreateUserWidget->SetItemHave(0);    // �÷��̾ ������ �ִ� ������ ���� ����
            }

            CurrentCraftItemRequireSlotArr.Add(CraftItemRequireSlotCreateUserWidget);   // �迭 �ȿ� ����
        }
        else {
            UE_LOG(LogTemp, Log, TEXT("CraftItemRequireSlotCreateUserWidget Create Fail"));
            FString Message = FString::Printf(TEXT("CraftItemRequireSlotCreateUserWidget Create Fail"));
            GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, Message);
        }
    }
    else {
        UE_LOG(LogTemp, Log, TEXT("CraftItemRequireSlotUserWidgetClass Call Fail"));
        FString Message = FString::Printf(TEXT("CraftItemRequireSlotUserWidgetClass Call Fail"));
        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, Message);
    }

}

