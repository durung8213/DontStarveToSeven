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
	/// �ϴ°� ����
	/// </summary>
	virtual void NativeConstruct() override;

	/// <summary>
	/// Info������ �ִ� ������ ������(�ʱ�ȭ�� �����)
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void ClearInfo();

	/// <summary>
	/// ������ �������� ��ҵ��� �Ű������� �޾ƿ� ǥ��
	/// </summary>
	/// <param name="ItemImage">������ ������ �̹���</param>
	/// <param name="ItemName">������ ������ �̸�</param>
	/// <param name="ItemRequireStructure">������ ���ۿ� �ʿ��� ������</param>
	/// <param name="ItemExplain">������ ������ ����</param>
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
