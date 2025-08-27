// Fill out your copyright notice in the Description page of Project Settings.


#include "Kimmh/HUDnUI/UI/DTS_CreaftTypeButtonUserWidget.h"
#include "Kimmh/Enum/CreaftingKindEnum.h"
#include "Components/Button.h"

/// <summary>
/// 버튼 바인드
/// </summary>
void UDTS_CreaftTypeButtonUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (Button) {
		// 바인드 함수 바인드
		Button->OnClicked.AddDynamic(this, &UDTS_CreaftTypeButtonUserWidget::OnButtonClick);
		Button_0->OnClicked.AddDynamic(this, &UDTS_CreaftTypeButtonUserWidget::OnButton_0Click);
		Button_1->OnClicked.AddDynamic(this, &UDTS_CreaftTypeButtonUserWidget::OnButton_1Click);
		Button_2->OnClicked.AddDynamic(this, &UDTS_CreaftTypeButtonUserWidget::OnButton_2Click);
		Button_3->OnClicked.AddDynamic(this, &UDTS_CreaftTypeButtonUserWidget::OnButton_3Click);
		Button_4->OnClicked.AddDynamic(this, &UDTS_CreaftTypeButtonUserWidget::OnButton_4Click);
		Button_5->OnClicked.AddDynamic(this, &UDTS_CreaftTypeButtonUserWidget::OnButton_5Click);
	}
}

/// <summary>
/// Favourite 버튼을 눌렀을 때 바인딩할 함수
/// </summary>
void UDTS_CreaftTypeButtonUserWidget::OnButtonClick()
{
	ConfirmButtonClicked(ECreaftingKind::Favourite);
}

/// <summary>
/// Item 버튼을 눌렀을 때 바인딩할 함수
/// </summary>
void UDTS_CreaftTypeButtonUserWidget::OnButton_0Click()
{
	ConfirmButtonClicked(ECreaftingKind::Item);
}

/// <summary>
/// Common 버튼을 눌렀을 때 바인딩할 함수
/// </summary>
void UDTS_CreaftTypeButtonUserWidget::OnButton_1Click()
{
	ConfirmButtonClicked(ECreaftingKind::Common);
}

/// <summary>
/// Tool 버튼을 눌렀을 때 바인딩할 함수
/// </summary>
void UDTS_CreaftTypeButtonUserWidget::OnButton_2Click()
{
	ConfirmButtonClicked(ECreaftingKind::Tool);
}

/// <summary>
/// Weapon 버튼을 눌렀을 때 바인딩할 함수
/// </summary>
void UDTS_CreaftTypeButtonUserWidget::OnButton_3Click()
{
	ConfirmButtonClicked(ECreaftingKind::Weapon);
}

/// <summary>
/// Turret 버튼을 눌렀을 때 바인딩할 함수
/// </summary>
void UDTS_CreaftTypeButtonUserWidget::OnButton_4Click()
{
	ConfirmButtonClicked(ECreaftingKind::Turret);
}

/// <summary>
/// Structures 버튼을 눌렀을 때 바인딩할 함수
/// </summary>
void UDTS_CreaftTypeButtonUserWidget::OnButton_5Click()
{
	ConfirmButtonClicked(ECreaftingKind::Structures);
}

/// <summary>
/// UDTS_CreaftingUserWidget에 그리드에 나타내야 할 것이 바뀐다는 것을 알려주는 델리게이트를 호출하는 함수
/// </summary>
/// <param name="CreaftngKind">변경될 크래프팅 종류</param>
void UDTS_CreaftTypeButtonUserWidget::ConfirmButtonClicked(ECreaftingKind CreaftngKind)
{
	OnConfirmClicked.Broadcast(CreaftngKind);	// 그리드에 나타내야 할 것이 바뀐다는 것을 알려주는 델리게이트를 호출
}



