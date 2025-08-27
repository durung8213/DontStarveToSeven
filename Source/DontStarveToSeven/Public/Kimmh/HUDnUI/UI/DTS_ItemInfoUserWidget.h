// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kimmh/HUDnUI/UI/DTS_UserWidget.h"
#include "Leesh/SHTotalStruct.h"
#include "DTS_ItemInfoUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class DONTSTARVETOSEVEN_API UDTS_ItemInfoUserWidget : public UDTS_UserWidget
{
	GENERATED_BODY()
	
public:
	/// <summary>
	/// �ϴ°� ����
	/// </summary>
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "ItemInfoUserWidget")
	void OnVisible();

	UFUNCTION(BlueprintCallable, Category = "ItemInfoUserWidget")
	void OnCollapsed();

	/// <summary>
	/// ������ ����â�� �������� �����ϴ� �Լ�
	/// </summary>
	/// <param name="RowName">������ RowName</param>
	void SetItem(FString RowName);

	/// <summary>
	/// ������ ����â�� �������� ����� �����ϴ� �Լ� (����â �� �κ� ���� ����)
	/// </summary>
	/// <param name="ItemGrade">������ ���</param>
	void SetItemGrade(EItemGrade ItemGrade);

	/// <summary>
	/// ������ ���� â�� ������ �̸��� �����ϴ� �Լ�
	/// </summary>
	/// <param name="ItemName">������ �̸�</param>
	void SetItemName(FText ItemName);

	/// <summary>
	/// ������ ���� â�� ������ Ÿ���� �����ϴ� �Լ�
	/// </summary>
	/// <param name="ItemTpye">������ Ÿ��</param>
	void SetItemType(ETestItemType ItemTpye);

	/// <summary>
	/// ������ ���� â�� ������ ������ �����ϴ� �Լ�
	/// </summary>
	/// <param name="ItemDescription">������ ����</param>
	void SetItemExplain(FText ItemDescription, FTestItemStatChanges ItemStatChanges);

	/// <summary>
	/// ������ ���� â�� �Լ� ��θ� �����ϴ� �Լ�
	/// </summary>
	/// <param name="ItemDropPath">������ ����</param>
	void SetItemDropPath(FText ItemDropPath);
public:
	/// <summary>
	/// ������ ����� ��Ÿ���� �̹��� â (���� ����� ����)
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	class UImage* ItemGradeBackground;

	/// <summary>
	/// ������ �̸� �ý�Ʈ ���
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemNameTextBlock;

	/// <summary>
	/// ������ Ÿ�� �ý�Ʈ ���
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemTypeTextBlock;

	/// <summary>
	/// ������ ���� �ؽ�Ʈ ���
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemExplainTextBlock;

	/// <summary>
	/// ������ ȹ�� ��� �ý�Ʈ ���
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemDropPathTextBlock;


};
