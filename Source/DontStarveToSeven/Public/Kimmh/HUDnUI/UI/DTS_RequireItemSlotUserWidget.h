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
	/// ItemAmountText �� ����
	/// </summary>
	/// <param name="ItemAmount">�ؽ�Ʈ�� ���� ��</param>
	void SetItemAmount(int ItemAmount);

	/// <summary>
	/// ItemNameText �� ����
	/// </summary>
	/// <param name="ItemName"></param>
	void SetItemName(FText ItemName);

	/// <summary>
	/// ItemCountTotalText �� ����
	/// </summary>
	/// <param name="ItemTotal"></param>
	void SetItemCountTotal(int ItemTotal);
	
	/// <summary>
	/// ItemHaveText �� ����
	/// </summary>
	/// <param name="ItemHave"></param>
	void SetItemHave(int ItemHave);

public:
	/// <summary>
	/// ������ 1�� ���۽� �ʿ��� ������ ��� �ý�Ʈ �ڽ�
	/// </summary>
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Creafting")
	class UTextBlock* ItemAmountText;
	
	/// <summary>
	/// ������ �̸� �ý�Ʈ �ڽ�
	/// </summary>
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Creafting")
	class UTextBlock* ItemNameText;

	/// <summary>
	/// ���ۿ� �ʿ��� ������ �� ������ ��� �ý�Ʈ �ڽ�
	/// </summary>
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Creafting")
	class UTextBlock* ItemCountTotalText;

	/// <summary>
	/// ���� ������ �ִ� ������ ������ ��� �ý�Ʈ �ڽ�
	/// </summary>
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Creafting")
	class UTextBlock* ItemHaveText;

	/// <summary>
	/// 1�� ���ۿ� �ʿ��� ������ ����
	/// </summary>
	int RequireItemCount;
	
	int TotalRequireItemCount;

	FString ResourceItemName;
};
