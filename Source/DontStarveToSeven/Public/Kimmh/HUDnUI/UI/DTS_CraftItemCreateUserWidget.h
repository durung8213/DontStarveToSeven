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
	/// �� ��ư ���ε�
	/// </summary>
	virtual void NativeConstruct() override;

	/// <summary>
	/// MakeCountTextBox�� ������ �ٲ�� �ش� ���� �������� Ȯ���ϰ� 
	/// ���ڸ� ���� ���ڰ� �ƴϸ� �������� �ʰ� �ϴ� �Լ�
	/// </summary>
	/// <param name="Text">�ٲ� ����</param>
	UFUNCTION()
	void OnMakeCountTextBoxChanged(const FText& Text);

	/// <summary>
	/// MakeCountTextBox�� ���ڸ� 1 ���� �Լ�
	/// </summary>
	UFUNCTION()
	void OnSubButtonClick();

	/// <summary>
	/// MakeCountTextBox�� ���ڸ� 1 ���ϴ� �Լ�
	/// </summary>
	UFUNCTION()
	void OnPlusButtonClick();

	/// <summary>
	/// MakeCountTextBox�� ���ڸ� ���� �� �ִ� �ִ� ������ �ø��� �Լ�
	/// </summary>
	UFUNCTION()
	void OnMaxButtonClick();

	/// <summary>
	/// ���� ��ư
	/// </summary>
	UFUNCTION()
	void OnCraftButtonClick();

	/// <summary>
	/// �ʿ��� ������ ���� ������ �߰��� ���� �ڽ��� ���� �Լ�
	/// </summary>
	UFUNCTION()
	void ClearNeedItemSlotVerticalBox();

	/// <summary>
	/// �� UI�� �Ⱥ��̰� �ϴ� �Լ�
	/// </summary>
	UFUNCTION()
	void CollispedCreafting();

	/// <summary>
	/// �� UI�� ���̰� �ϴ� �Լ�
	/// </summary>
	UFUNCTION()
	void VisibleCreafting();

	UFUNCTION()
	void SetCraftStructureType(ECraftType NewCreaftingType);

	void ReloadRequireItemCount();

	/// <summary>
	/// ���ۿ� �ʿ��� ������ ������ ����� �Լ�
	/// </summary>
	/// <param name="ResourceItemName">�ʿ��� ������ �̸�</param>
	/// <param name="ResoucreItemCount">�ʿ��� ������ ����</param>
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
