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
	/// UI�� �ҷ������� ��ư ���ε�
	/// </summary>
	virtual void NativeConstruct() override;


	/// <summary>
	/// ���� ��� ���Կ� ���� �̹��� ����
	/// </summary>
	/// <param name="Image">���� �̹���</param>
	UFUNCTION(BlueprintCallable)
	void SetCraftingImage(UTexture2D* Image);

	/// <summary>
	/// CraftItemInfo�� ��� ���� ���� ��������Ʈ�� ȣ���ϴ� �Լ�
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void CallCraftingInfo();

public:
	// ��ư�� Ŭ�� �Ǿ��� �� �׸��� �������� �� ������ ũ������ ������ RowName�� �����ϴ� ��������Ʈ
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
