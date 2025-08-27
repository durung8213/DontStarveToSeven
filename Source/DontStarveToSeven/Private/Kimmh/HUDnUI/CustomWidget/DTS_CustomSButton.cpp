// Fill out your copyright notice in the Description page of Project Settings.


#include "Kimmh/HUDnUI/CustomWidget/DTS_CustomSButton.h"



FReply DTS_CustomSButton::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	/*FReply Reply = FReply::Unhandled();
	Reply.ReleaseMouseCapture();
	return FReply::Handled().ReleaseMouseCapture();*/

	FReply Reply = SButton::OnMouseButtonDown(MyGeometry, MouseEvent);
	Reply.ReleaseMouseCapture();  // ���콺 ĸó ����
	Press();
	return Reply;
}

FReply DTS_CustomSButton::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	// ���콺�� ��ư���� ���� �� ȣ���
	FReply Reply = SButton::OnMouseButtonUp(MyGeometry, MouseEvent);

	// ���콺 ĸó ���� (�ʿ��� ���)
	Reply.ReleaseMouseCapture();

	return Reply;
}


