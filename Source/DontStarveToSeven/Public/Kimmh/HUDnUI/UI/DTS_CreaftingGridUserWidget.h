// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kimmh/HUDnUI/UI/DTS_UserWidget.h"
#include "Kimmh/Enum/CreaftingKindEnum.h"
#include "DTS_CreaftingGridUserWidget.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGridPanelDelegate, ECreaftingKind, CreaftingKind, FName, RowName);

UCLASS()
class DONTSTARVETOSEVEN_API UDTS_CreaftingGridUserWidget : public UDTS_UserWidget
{
	GENERATED_BODY()
	
public:
	/// <summary>
	/// ������
	/// ������ ���̺��� �ҷ��� �����Ѵ�
	/// </summary>
	/// <param name="ObjectInitializer"></param>
	UDTS_CreaftingGridUserWidget(const FObjectInitializer& ObjectInitializer);
	
	/// <summary>
	/// UI�� AddToViewport�� �Ǹ� CreateCreaftingSlots �����Ѵ�
	/// </summary>
	virtual void NativeConstruct() override;
	

	/// <summary>
	/// Kind �´� ũ������ ��ư ���Ե��� ������ ������ ���̺���� ��� ����� �Լ��� �����Ѵ�
	/// </summary>
	/// <param name="Kind">ũ������ �з�</param>
	void CreateCreaftingSlots(ECreaftingKind Kind);

	/// <summary>
	/// ũ������ ������ ����� CreaftingGridPanel�� �ٿ��ִ� �Լ�
	/// </summary>
	/// <param name="RowName">������ ���̺��� RowName</param>
	/// <param name="CraftingKind">����� ����</param>
	/// <param name="CreaftingImage">���Կ� ��Ÿ�� �̹���</param>
	/// <param name="SlotNum">���� ��ȣ</param>
	void CreateCraftingSlot(FName RowName, ECreaftingKind CraftingKind, UTexture2D* CreaftingImage, int SlotNum);


	/// <summary>
	/// UDTS_CraftingSlotUserWidget���� ���� ��������Ʈ�� ���ε��� �Լ�
	/// </summary>
	/// <param name="CreaftingKind">ũ������ ����</param>
	/// <param name="RowName">������ ���̺��� �̸�</param>
	UFUNCTION()
	void CallCreaftingInfo(ECreaftingKind CreaftingKind, FName RowName);
public:
	// ũ������ ���Կ��� �޾ƿ� ������ CraftingUserWidget���� ������ ��������Ʈ
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnGridPanelDelegate OnGridPanelDelegate;

	/// <summary>
	/// UDTS_CraftingSlotUserWidget�� �ٿ��� �׸��� �г�
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* CreaftingGridPanel;
	
	/// <summary>
	/// ���� �׸��� �гο� ��Ÿ���� �ִ� ũ������ ����
	/// </summary>
	ECreaftingKind CurrentKind;

	//UDataTable* FavouriteCraftingDataTable;

	// ������ ���̺��

	UDataTable* ItemCraftingDataTable;

	UDataTable* CommonCraftingDataTable;

	UDataTable* ToolCraftingDataTable;

	UDataTable* WeaponCraftingDataTable;

	UDataTable* TurretCraftingDataTable;

	UDataTable* StructuresCraftingDataTable;
};
