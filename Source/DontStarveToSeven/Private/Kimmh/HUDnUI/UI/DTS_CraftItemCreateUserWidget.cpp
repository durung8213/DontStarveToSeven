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
    );  // 크래프팅 개수를 나타낼 에디터블 택스트 박스의 내용이 변했을 때 실행하는 함수 바인드

    SubButton->OnClicked.AddDynamic(
        this, 
        &UDTS_CraftItemCreateUserWidget::OnSubButtonClick
    );  // 클릭 함수 바인드

    PlusButton->OnClicked.AddDynamic(
        this, 
        &UDTS_CraftItemCreateUserWidget::OnPlusButtonClick
    );  // 클릭 함수 바인드

    MaxButton->OnClicked.AddDynamic(
        this, 
        &UDTS_CraftItemCreateUserWidget::OnMaxButtonClick
    );  // 클릭 함수 바인드

    CraftButton->OnClicked.AddDynamic(
        this, 
        &UDTS_CraftItemCreateUserWidget::OnCraftButtonClick
    );  // 클릭 함수 바인드
}

/// <summary>
/// MakeCountTextBox의 내용이 바뀌면 해당 것이 숫자인지 확인하고 
/// 숫자면 적용 숫자가 아니면 적용하지 않게 하는 함수
/// </summary>
/// <param name="Text">바뀐 글자</param>
void UDTS_CraftItemCreateUserWidget::OnMakeCountTextBoxChanged(const FText& Text)
{
    FString FilteredText;
    for (TCHAR Character : Text.ToString())
    {
        if (FChar::IsDigit(Character))  // 숫자인 경우에만 추가
        {
            // 숫자가 맞으면 적용 택스트에 더함
            FilteredText.AppendChar(Character);
        }
    }

    // 택스트 적용
    if (FilteredText != Text.ToString())
    {
        MakeCountTextBox->SetText(FText::FromString(FilteredText));
    }

}


/// <summary>
/// MakeCountTextBox의 숫자를 1 빼는 함수
/// </summary>
void UDTS_CraftItemCreateUserWidget::OnSubButtonClick()
{
    FString TextString = MakeCountTextBox->GetText().ToString();
    if (TextString.IsNumeric()) {
        // 이 부분은 바로 밑 함수에 있는거랑 73번째 줄 제외 완전히 같으니 그것을 볼 것
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
        UKismetSystemLibrary::PrintString(this, TEXT("생성 카운트 택스트 상자의 내용을 숫자로 변경 불가"));
    }

}

/// <summary>
/// MakeCountTextBox의 숫자를 1 더하는 함수
/// </summary>
void UDTS_CraftItemCreateUserWidget::OnPlusButtonClick()
{
    FString TextString = MakeCountTextBox->GetText().ToString();
    if (TextString.IsNumeric()) {
        // 에디터블 택스트 박스에 있는 것이 숫자면
        int32 Count = FCString::Atoi(*MakeCountTextBox->GetText().ToString());  // int32로 형변환
        Count++;    // 증가시킴
       
        MakeCountTextBox->SetText(FText::AsNumber(Count));  // 에디터블 택스트에 글자 설정
        for (UDTS_RequireItemSlotUserWidget* RequireItemSlot : CurrentCraftItemRequireSlotArr) {
            RequireItemSlot
                ->SetItemCountTotal
                (
                    RequireItemSlot->RequireItemCount * Count
                );  // 필요 개수 * 만든 갯수로 슬롯 설정
        }
    }
    else {
        UE_LOG(LogTemp, Log, TEXT("생성 카운트 택스트 상자의 내용을 숫자로 변경 불가"));
        FString Message = FString::Printf(TEXT("생성 카운트 택스트 상자의 내용을 숫자로 변경 불가"));
        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, Message);
    }
}

/// <summary>
/// MakeCountTextBox의 숫자를 만들 수 있는 최대 갯수로 올리는 함수
/// </summary>
void UDTS_CraftItemCreateUserWidget::OnMaxButtonClick()
{
    // 는 인벤토리 담당이랑 연결해야 함
    UE_LOG(LogTemp, Log, TEXT("Inventory Marge Require"));
    FString Message = FString::Printf(TEXT("Inventory Marge Require"));
    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, Message);
}

/// <summary>
/// 제작 버튼
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
        // 아이템이면 (인벤토리 담당과 연걸..하는게 필요)
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
        // 건축물이면
        CurrentPlayerController = Cast<ADTS_GamePlayPlayerController>(GetOwningPlayer());
        if (CurrentPlayerController) {
            // 플레이어 컨트롤러를 가져오고 IMC를 제작모드로 변경
            CurrentPlayerController->ChangeInputMappingContext(EIMC::CreaftingIMC);
            CurrentPlayer = Cast<ABasePlayerCharacter>(CurrentPlayerController->GetPawn());
            if (CurrentPlayer) {
                // 크래프팅 메니저에 ~를 만들 것이라고 전달
                // 주연 수정
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
/// 필요한 아이템 슬롯 위젯이 추가될 세로 박스를 비우는 함수
/// </summary>
void UDTS_CraftItemCreateUserWidget::ClearNeedItemSlotVerticalBox()
{
    if(NeedItemSlotVerticalBox)
        // 버티컬 박스 내 자식들을 없엠
        NeedItemSlotVerticalBox->ClearChildren();

    if (!CurrentCraftItemRequireSlotArr.IsEmpty()) {
        // 슬롯 배열 비움
        CurrentCraftItemRequireSlotArr.Empty();
    }
}

/// <summary>
/// 이 UI가 안보이게 하는 함수
/// </summary>
void UDTS_CraftItemCreateUserWidget::CollispedCreafting()
{
    // 있는 모든 것을 안보이게 설정
    SubButton->SetVisibility(ESlateVisibility::Collapsed);
    PlusButton->SetVisibility(ESlateVisibility::Collapsed);
    MaxButton->SetVisibility(ESlateVisibility::Collapsed);
    CraftButton->SetVisibility(ESlateVisibility::Collapsed);
    MakeCountTextBox->SetText(FText::AsNumber(1));
    MakeCountTextBox->SetVisibility(ESlateVisibility::Collapsed);

    // 재료 아이템 슬롯 박스를 비우는 함수 실행
    ClearNeedItemSlotVerticalBox();
}

/// <summary>
/// 이 UI가 보이게 하는 함수
/// </summary>
void UDTS_CraftItemCreateUserWidget::VisibleCreafting()
{
    // 있는 모든 것을 보이게 설정
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
                CraftItemRequireSlot->SetItemHave(0);    // 플레이어가 가지고 있는 아이템 개수 설정
            }
        }
    }
    
}

/// <summary>
/// 제작에 필요한 아이템 슬롯을 만드는 함수
/// </summary>
/// <param name="ResourceItemName">필요한 아이템 이름</param>
/// <param name="ResoucreItemCount">필요한 아이템 개수</param>
void UDTS_CraftItemCreateUserWidget::CreateRequireItemSlot(FText ResourceItemName, int ResoucreItemCount)
{
    /*TSubclassOf<UDTS_RequireItemSlotUserWidget> CraftItemRequireSlotUserWidgetClass;

    CraftItemRequireSlotUserWidgetClass = LoadClass<UDTS_RequireItemSlotUserWidget>(nullptr, TEXT("/Game/Kimmh/UI/Creafting/WBP_RequireItemSlot.WBP_RequireItemSlot_C"));*/

    FSoftObjectPath WidgetPath(TEXT("/Game/Kimmh/UI/Creafting/WBP_RequireItemSlot.WBP_RequireItemSlot_C"));

    UClass* CraftItemRequireSlotUserWidgetClass = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), nullptr, *WidgetPath.ToString()));

    if (CraftItemRequireSlotUserWidgetClass) {
        UDTS_RequireItemSlotUserWidget* CraftItemRequireSlotCreateUserWidget = CreateWidget<UDTS_RequireItemSlotUserWidget>(GetWorld()->GetFirstPlayerController(), CraftItemRequireSlotUserWidgetClass);
        if (CraftItemRequireSlotCreateUserWidget) {
            // 위젯 생성을 성공하면

            NeedItemSlotVerticalBox->AddChildToVerticalBox(CraftItemRequireSlotCreateUserWidget);   // 위젯을 버티컬 박스 자식으로 넣음
            CraftItemRequireSlotCreateUserWidget->SetItemAmount(ResoucreItemCount); // 필요 아이템 개수(택스트박스내, 1개 기준) 설정
            CraftItemRequireSlotCreateUserWidget->RequireItemCount = ResoucreItemCount; // 필요 아이템 개수(변수로, 1개 기준) 설정
            CraftItemRequireSlotCreateUserWidget->SetItemName(ResourceItemName);   // 필요 아이템 이름 설정
            CraftItemRequireSlotCreateUserWidget->SetItemCountTotal(ResoucreItemCount * FCString::Atoi(*MakeCountTextBox->GetText().ToString())); // 필요 아이템 개수 * 제작할 아이템 개수

            ADTS_GamePlayPlayerController* CurrentPlayerController = Cast<ADTS_GamePlayPlayerController>(GetOwningPlayer());
            if (CurrentPlayerController) {
                CraftItemRequireSlotCreateUserWidget->SetItemHave(
                    CurrentPlayerController->SearchItemCountToInventory(ResourceItemName.ToString())
                );
            }
            else {
                CraftItemRequireSlotCreateUserWidget->SetItemHave(0);    // 플레이어가 가지고 있는 아이템 개수 설정
            }

            CurrentCraftItemRequireSlotArr.Add(CraftItemRequireSlotCreateUserWidget);   // 배열 안에 넣음
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

