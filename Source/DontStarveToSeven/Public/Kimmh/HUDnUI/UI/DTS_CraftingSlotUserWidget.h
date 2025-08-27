// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kimmh/HUDnUI/UI/DTS_UserWidget.h"
#include "Kimmh/Enum/CreaftingKindEnum.h"
#include "Blueprint/UserWidget.h"
#include "DTS_CraftingSlotUserWidget.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCraftSlotButton, ECreaftingKind, CreaftingKind, FName, RowName);

UCLASS()
class DONTSTARVETOSEVEN_API UDTS_CraftingSlotUserWidget : public UDTS_UserWidget
{
	GENERATED_BODY()
public:

	/// <summary>
	/// UI가 불러와지면 버튼 바인드
	/// </summary>
	virtual void NativeConstruct() override;


	/// <summary>
	/// 제작 목록 슬롯에 있을 이미지 설정
	/// </summary>
	/// <param name="Image">슬롯 이미지</param>
	UFUNCTION(BlueprintCallable)
	void SetCraftingImage(UTexture2D* Image);

	/// <summary>
	/// CraftItemInfo에 띄울 정보 전달 델리게이트를 호출하는 함수
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void CallCraftingInfo();

public:
	// 버튼이 클릭 되었을 떄 그리드 위젯으로 이 슬롯의 크래프팅 종류와 RowName을 전달하는 델리게이트
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCraftSlotButton OnButtonClicked;

	UPROPERTY(BlueprintReadOnly)
	FName RowName;

	UPROPERTY(BlueprintReadOnly)
	ECreaftingKind CraftingKind;

	UPROPERTY(meta = (BindWidget))
	class UImage* CraftingImage;

	UPROPERTY(meta = (BindWidget))
	class UButton* CreaftingSlotButton;
};
