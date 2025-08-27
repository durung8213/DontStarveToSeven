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
	/// 크래프팅 타입 버튼 위젯 생성 및 붙여주는 함수
	/// </summary>
	void CreateCreaftingTypeButtonWidget();

	/// <summary>
	/// 크래프팅 그리드 위젯 생성 및 붙여주는 함수
	/// </summary>
	void CreateCreaftingGridWidget();

	/// <summary>
	/// 크래프팅 아이템 정보 위젯 생성 및 붙여주는 함수
	/// </summary>
	void CreateCraftingInfo();

	/// <summary>
	/// 크래프팅 아이템 제작 요구 아이템 위젯 생성 및 붙여주는 함수
	/// </summary>
	void CreateCraftingItemCreateWidget();

	/// <summary>
	/// 크래프팅 종류가 바뀌었다는 것을 알리는 
	/// UDTS_CreaftTypeButtonUserWidget의 델리게이트에 바인딩 될 함수
	/// </summary>
	/// <param name="CreaftingKind">바뀔 종류 Enum</param>
	UFUNCTION()
	void ChangeCreaftingGridDT(ECreaftingKind CreaftingKind);

	/// <summary>
	/// 생성할 아이템을 정했다는 것을 알리는 
	/// UDTS_CreaftingGridUserWidget의 델리게이트를 바인딩함 
	/// </summary>
	/// <param name="CreaftingKind">아이템 분류</param>
	/// <param name="RowName">아이템 이름</param>
	UFUNCTION()
	void ChangeCreaftingInfoUI(ECreaftingKind CreaftingKind, FName RowName);

	void ReloadRequireItemCount();

public:
	/*
		보더는 위젯을 붙일 장소
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
	/// 크래프팅 그리드 위젯 변슈
	/// </summary>
	UPROPERTY()
	class UDTS_CreaftingGridUserWidget* CreaftingGridUserWidget;

	/// <summary>
	/// 크래프팅 정도 위젯 변수
	/// </summary>
	UPROPERTY()
	class UDTS_CraftingInfoUserWidget* CraftingInfoUserWidget;

	/// <summary>
	/// 크래프팅 아이템 제작 위젯 변수
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
