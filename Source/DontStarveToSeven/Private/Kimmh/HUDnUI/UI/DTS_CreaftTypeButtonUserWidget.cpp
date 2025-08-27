// Fill out your copyright notice in the Description page of Project Settings.


#include "Kimmh/HUDnUI/UI/DTS_CreaftTypeButtonUserWidget.h"
#include "Kimmh/Enum/CreaftingKindEnum.h"
#include "Components/Button.h"

/// <summary>
/// ��ư ���ε�
/// </summary>
void UDTS_CreaftTypeButtonUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (Button) {
		// ���ε� �Լ� ���ε�
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
/// Favourite ��ư�� ������ �� ���ε��� �Լ�
/// </summary>
void UDTS_CreaftTypeButtonUserWidget::OnButtonClick()
{
	ConfirmButtonClicked(ECreaftingKind::Favourite);
}

/// <summary>
/// Item ��ư�� ������ �� ���ε��� �Լ�
/// </summary>
void UDTS_CreaftTypeButtonUserWidget::OnButton_0Click()
{
	ConfirmButtonClicked(ECreaftingKind::Item);
}

/// <summary>
/// Common ��ư�� ������ �� ���ε��� �Լ�
/// </summary>
void UDTS_CreaftTypeButtonUserWidget::OnButton_1Click()
{
	ConfirmButtonClicked(ECreaftingKind::Common);
}

/// <summary>
/// Tool ��ư�� ������ �� ���ε��� �Լ�
/// </summary>
void UDTS_CreaftTypeButtonUserWidget::OnButton_2Click()
{
	ConfirmButtonClicked(ECreaftingKind::Tool);
}

/// <summary>
/// Weapon ��ư�� ������ �� ���ε��� �Լ�
/// </summary>
void UDTS_CreaftTypeButtonUserWidget::OnButton_3Click()
{
	ConfirmButtonClicked(ECreaftingKind::Weapon);
}

/// <summary>
/// Turret ��ư�� ������ �� ���ε��� �Լ�
/// </summary>
void UDTS_CreaftTypeButtonUserWidget::OnButton_4Click()
{
	ConfirmButtonClicked(ECreaftingKind::Turret);
}

/// <summary>
/// Structures ��ư�� ������ �� ���ε��� �Լ�
/// </summary>
void UDTS_CreaftTypeButtonUserWidget::OnButton_5Click()
{
	ConfirmButtonClicked(ECreaftingKind::Structures);
}

/// <summary>
/// UDTS_CreaftingUserWidget�� �׸��忡 ��Ÿ���� �� ���� �ٲ�ٴ� ���� �˷��ִ� ��������Ʈ�� ȣ���ϴ� �Լ�
/// </summary>
/// <param name="CreaftngKind">����� ũ������ ����</param>
void UDTS_CreaftTypeButtonUserWidget::ConfirmButtonClicked(ECreaftingKind CreaftngKind)
{
	OnConfirmClicked.Broadcast(CreaftngKind);	// �׸��忡 ��Ÿ���� �� ���� �ٲ�ٴ� ���� �˷��ִ� ��������Ʈ�� ȣ��
}



