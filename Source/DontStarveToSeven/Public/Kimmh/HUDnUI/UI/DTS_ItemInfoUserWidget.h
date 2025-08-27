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
	/// 하는거 없음
	/// </summary>
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "ItemInfoUserWidget")
	void OnVisible();

	UFUNCTION(BlueprintCallable, Category = "ItemInfoUserWidget")
	void OnCollapsed();

	/// <summary>
	/// 아이템 정보창에 아이템을 세팅하는 함수
	/// </summary>
	/// <param name="RowName">아이템 RowName</param>
	void SetItem(FString RowName);

	/// <summary>
	/// 아이템 정보창에 아이템의 등급을 세팅하는 함수 (정보창 윗 부분 색이 변경)
	/// </summary>
	/// <param name="ItemGrade">아이템 등급</param>
	void SetItemGrade(EItemGrade ItemGrade);

	/// <summary>
	/// 아이템 정보 창에 아이템 이름을 설정하는 함수
	/// </summary>
	/// <param name="ItemName">아이템 이름</param>
	void SetItemName(FText ItemName);

	/// <summary>
	/// 아이템 정보 창에 아이템 타입을 설정하는 함수
	/// </summary>
	/// <param name="ItemTpye">아이템 타입</param>
	void SetItemType(ETestItemType ItemTpye);

	/// <summary>
	/// 아이템 정보 창에 아이템 설명을 설정하는 함수
	/// </summary>
	/// <param name="ItemDescription">아이템 설명</param>
	void SetItemExplain(FText ItemDescription, FTestItemStatChanges ItemStatChanges);

	/// <summary>
	/// 아이템 정보 창에 입수 경로를 설정하는 함수
	/// </summary>
	/// <param name="ItemDropPath">아이템 설명</param>
	void SetItemDropPath(FText ItemDropPath);
public:
	/// <summary>
	/// 아이템 등급을 나타내는 이미지 창 (색만 변경될 예정)
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	class UImage* ItemGradeBackground;

	/// <summary>
	/// 아이템 이름 택스트 블록
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemNameTextBlock;

	/// <summary>
	/// 아이템 타입 택스트 블록
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemTypeTextBlock;

	/// <summary>
	/// 아이템 설명 텍스트 블록
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemExplainTextBlock;

	/// <summary>
	/// 아이템 획득 경로 택스트 블록
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemDropPathTextBlock;


};
