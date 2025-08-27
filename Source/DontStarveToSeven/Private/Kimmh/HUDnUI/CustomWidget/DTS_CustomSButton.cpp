// Fill out your copyright notice in the Description page of Project Settings.


#include "Kimmh/HUDnUI/CustomWidget/DTS_CustomSButton.h"



FReply DTS_CustomSButton::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	/*FReply Reply = FReply::Unhandled();
	Reply.ReleaseMouseCapture();
	return FReply::Handled().ReleaseMouseCapture();*/

	FReply Reply = SButton::OnMouseButtonDown(MyGeometry, MouseEvent);
	Reply.ReleaseMouseCapture();  // 마우스 캡처 해제
	Press();
	return Reply;
}

FReply DTS_CustomSButton::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	// 마우스를 버튼에서 뗐을 때 호출됨
	FReply Reply = SButton::OnMouseButtonUp(MyGeometry, MouseEvent);

	// 마우스 캡처 해제 (필요한 경우)
	Reply.ReleaseMouseCapture();

	return Reply;
}


