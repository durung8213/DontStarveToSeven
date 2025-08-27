// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kimmh/HUDnUI/UI/DTS_UserWidget.h"
#include "DTS_RequireItemSlotUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class DONTSTARVETOSEVEN_API UDTS_RequireItemSlotUserWidget : public UDTS_UserWidget
{
	GENERATED_BODY()

public:
	/// <summary>
	/// ItemAmountText 값 설정
	/// </summary>
	/// <param name="ItemAmount">텍스트에 넣을 값</param>
	void SetItemAmount(int ItemAmount);

	/// <summary>
	/// ItemNameText 값 설정
	/// </summary>
	/// <param name="ItemName"></param>
	void SetItemName(FText ItemName);

	/// <summary>
	/// ItemCountTotalText 값 설정
	/// </summary>
	/// <param name="ItemTotal"></param>
	void SetItemCountTotal(int ItemTotal);
	
	/// <summary>
	/// ItemHaveText 값 설정
	/// </summary>
	/// <param name="ItemHave"></param>
	void SetItemHave(int ItemHave);

public:
	/// <summary>
	/// 아이템 1개 제작시 필요한 개수를 담는 택스트 박스
	/// </summary>
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Creafting")
	class UTextBlock* ItemAmountText;
	
	/// <summary>
	/// 아이템 이름 택스트 박스
	/// </summary>
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Creafting")
	class UTextBlock* ItemNameText;

	/// <summary>
	/// 제작에 필요한 아이템 총 개수를 담는 택스트 박스
	/// </summary>
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Creafting")
	class UTextBlock* ItemCountTotalText;

	/// <summary>
	/// 내가 가지고 있는 아이탬 개수를 담는 택스트 박스
	/// </summary>
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Creafting")
	class UTextBlock* ItemHaveText;

	/// <summary>
	/// 1개 제작에 필요한 아이템 개수
	/// </summary>
	int RequireItemCount;
	
	int TotalRequireItemCount;

	FString ResourceItemName;
};
