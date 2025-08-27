// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kimmh/HUDnUI/UI/DTS_UserWidget.h"
#include "Kimmh/Enum/CreaftingTypeEnum.h"
#include "DTS_CraftItemCreateUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class DONTSTARVETOSEVEN_API UDTS_CraftItemCreateUserWidget : public UDTS_UserWidget
{
	GENERATED_BODY()
	
public:
	/// <summary>
	/// 각 버튼 바인드
	/// </summary>
	virtual void NativeConstruct() override;

	/// <summary>
	/// MakeCountTextBox의 내용이 바뀌면 해당 것이 숫자인지 확인하고 
	/// 숫자면 적용 숫자가 아니면 적용하지 않게 하는 함수
	/// </summary>
	/// <param name="Text">바뀐 글자</param>
	UFUNCTION()
	void OnMakeCountTextBoxChanged(const FText& Text);

	/// <summary>
	/// MakeCountTextBox의 숫자를 1 빼는 함수
	/// </summary>
	UFUNCTION()
	void OnSubButtonClick();

	/// <summary>
	/// MakeCountTextBox의 숫자를 1 더하는 함수
	/// </summary>
	UFUNCTION()
	void OnPlusButtonClick();

	/// <summary>
	/// MakeCountTextBox의 숫자를 만들 수 있는 최대 갯수로 올리는 함수
	/// </summary>
	UFUNCTION()
	void OnMaxButtonClick();

	/// <summary>
	/// 제작 버튼
	/// </summary>
	UFUNCTION()
	void OnCraftButtonClick();

	/// <summary>
	/// 필요한 아이템 슬롯 위젯이 추가될 세로 박스를 비우는 함수
	/// </summary>
	UFUNCTION()
	void ClearNeedItemSlotVerticalBox();

	/// <summary>
	/// 이 UI가 안보이게 하는 함수
	/// </summary>
	UFUNCTION()
	void CollispedCreafting();

	/// <summary>
	/// 이 UI가 보이게 하는 함수
	/// </summary>
	UFUNCTION()
	void VisibleCreafting();

	UFUNCTION()
	void SetCraftStructureType(ECraftType NewCreaftingType);

	void ReloadRequireItemCount();

	/// <summary>
	/// 제작에 필요한 아이템 슬롯을 만드는 함수
	/// </summary>
	/// <param name="ResourceItemName">필요한 아이템 이름</param>
	/// <param name="ResoucreItemCount">필요한 아이템 개수</param>
	UFUNCTION()
	void CreateRequireItemSlot(FText ResourceItemName, int ResoucreItemCount);

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Creafting")
	class UButton* SubButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Creafting")
	class UButton* PlusButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Creafting")
	class UButton* MaxButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Creafting")
	class UButton* CraftButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Creafting")
	class UEditableTextBox* MakeCountTextBox;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Creafting")
	class UVerticalBox* NeedItemSlotVerticalBox;

	UPROPERTY()
	TArray<class UDTS_RequireItemSlotUserWidget*> CurrentCraftItemRequireSlotArr;

	UPROPERTY()
	ECreaftingType CraftType;

	UPROPERTY()
	ECraftType CraftStructureType;

	UPROPERTY()
	FName CraftItemDBRowName;
};
