// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kimmh/HUDnUI/UI/DTS_UserWidget.h"
#include "DTS_CraftingInfoUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class DONTSTARVETOSEVEN_API UDTS_CraftingInfoUserWidget : public UDTS_UserWidget
{
	GENERATED_BODY()
public:
	/// <summary>
	/// 하는거 없음
	/// </summary>
	virtual void NativeConstruct() override;

	/// <summary>
	/// Info위젯에 있는 모든것을 지워줌(초기화에 가까움)
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void ClearInfo();

	/// <summary>
	/// 제작할 아이템의 요소들을 매개변수로 받아와 표시
	/// </summary>
	/// <param name="ItemImage">제작할 아이템 이미지</param>
	/// <param name="ItemName">제작할 아이템 이름</param>
	/// <param name="ItemRequireStructure">아이템 제작에 필요한 구조물</param>
	/// <param name="ItemExplain">제작할 아이템 설명</param>
	UFUNCTION(BlueprintCallable)
	void SetItemInfo(UTexture2D* ItemImage, FText ItemName, FText ItemRequireStructure, FText ItemExplain);


public:
	UPROPERTY(meta = (BindWidget))
	class UImage* CraftItemImage;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CraftItemName;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CraftItemRequireStructure;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CraftItemExplain;

	
};
