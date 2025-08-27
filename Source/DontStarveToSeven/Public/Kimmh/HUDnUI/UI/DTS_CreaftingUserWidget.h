// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kimmh/HUDnUI/UI/DTS_UserWidget.h"
#include "Kimmh/Enum/CreaftingKindEnum.h"
#include "DTS_CreaftingUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class DONTSTARVETOSEVEN_API UDTS_CreaftingUserWidget : public UDTS_UserWidget
{
	GENERATED_BODY()
	UDTS_CreaftingUserWidget();
public:
	virtual void NativeConstruct() override;

	/// <summary>
	/// ũ������ Ÿ�� ��ư ���� ���� �� �ٿ��ִ� �Լ�
	/// </summary>
	void CreateCreaftingTypeButtonWidget();

	/// <summary>
	/// ũ������ �׸��� ���� ���� �� �ٿ��ִ� �Լ�
	/// </summary>
	void CreateCreaftingGridWidget();

	/// <summary>
	/// ũ������ ������ ���� ���� ���� �� �ٿ��ִ� �Լ�
	/// </summary>
	void CreateCraftingInfo();

	/// <summary>
	/// ũ������ ������ ���� �䱸 ������ ���� ���� �� �ٿ��ִ� �Լ�
	/// </summary>
	void CreateCraftingItemCreateWidget();

	/// <summary>
	/// ũ������ ������ �ٲ���ٴ� ���� �˸��� 
	/// UDTS_CreaftTypeButtonUserWidget�� ��������Ʈ�� ���ε� �� �Լ�
	/// </summary>
	/// <param name="CreaftingKind">�ٲ� ���� Enum</param>
	UFUNCTION()
	void ChangeCreaftingGridDT(ECreaftingKind CreaftingKind);

	/// <summary>
	/// ������ �������� ���ߴٴ� ���� �˸��� 
	/// UDTS_CreaftingGridUserWidget�� ��������Ʈ�� ���ε��� 
	/// </summary>
	/// <param name="CreaftingKind">������ �з�</param>
	/// <param name="RowName">������ �̸�</param>
	UFUNCTION()
	void ChangeCreaftingInfoUI(ECreaftingKind CreaftingKind, FName RowName);

	void ReloadRequireItemCount();

public:
	/*
		������ ������ ���� ���
	*/

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Creafting")
	class UBorder* CreaftingGridBorder;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Creafting")
	class UBorder* CreaftingInfoBorder;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Creafting")
	class UBorder* CreaftingTypeButtonBorder;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Creafting")
	class UBorder* CreaftingNeedItem;

	/// <summary>
	/// ũ������ �׸��� ���� ����
	/// </summary>
	UPROPERTY()
	class UDTS_CreaftingGridUserWidget* CreaftingGridUserWidget;

	/// <summary>
	/// ũ������ ���� ���� ����
	/// </summary>
	UPROPERTY()
	class UDTS_CraftingInfoUserWidget* CraftingInfoUserWidget;

	/// <summary>
	/// ũ������ ������ ���� ���� ����
	/// </summary>
	UPROPERTY()
	class UDTS_CraftItemCreateUserWidget* CraftintItemCreateUserWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UDTS_CreaftTypeButtonUserWidget> CreaftingTypeButtonUserWidgetClass;	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UDTS_CreaftingGridUserWidget> CreaftingGridUserWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UDTS_CraftingInfoUserWidget> CreaftingInfoUserWidgetClass;	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UDTS_CraftItemCreateUserWidget> CraftintItemCreateUserWidgetClass;
};
