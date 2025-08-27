// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kimmh/HUDnUI/UI/DTS_UserWidget.h"
#include "Blueprint/UserWidget.h"
#include "DTS_CreaftTypeButtonUserWidget.generated.h"

/**
 * 
 */
 // 🔹 이벤트 디스패처 선언
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCreaftTypeButtonDelegate, ECreaftingKind, CreaftingKind);

UCLASS()
class DONTSTARVETOSEVEN_API UDTS_CreaftTypeButtonUserWidget : public UDTS_UserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;

	/// <summary>
	/// Favourite 버튼을 눌렀을 때 바인딩할 함수
	/// </summary>
	UFUNCTION()
	void OnButtonClick();

	/// <summary>
	/// Item 버튼을 눌렀을 때 바인딩할 함수
	/// </summary>
	UFUNCTION()
	void OnButton_0Click();
	
	/// <summary>
	/// Common 버튼을 눌렀을 때 바인딩할 함수
	/// </summary>
	UFUNCTION()
	void OnButton_1Click();
	
	/// <summary>
	/// Tool 버튼을 눌렀을 때 바인딩할 함수
	/// </summary>
	UFUNCTION()
	void OnButton_2Click();
	
	/// <summary>
	/// Weapon 버튼을 눌렀을 때 바인딩할 함수
	/// </summary>
	UFUNCTION()
	void OnButton_3Click();
	
	/// <summary>
	/// Turret 버튼을 눌렀을 때 바인딩할 함수
	/// </summary>
	UFUNCTION()
	void OnButton_4Click();
	
	/// <summary>
	/// Structures 버튼을 눌렀을 때 바인딩할 함수
	/// </summary>
	UFUNCTION()
	void OnButton_5Click();
	
	/// <summary>
	/// UDTS_CreaftingUserWidget에 그리드에 나타내야 할 것이 바뀐다는 것을 알려주는 델리게이트를 호출하는 함수
	/// </summary>
	/// <param name="CreaftngKind">변경될 크래프팅 종류</param>
	UFUNCTION()
	void ConfirmButtonClicked(ECreaftingKind CreaftngKind);

	

public:
	// 버튼이 눌렸다는 것을 나타내는 델리게이트
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FCreaftTypeButtonDelegate OnConfirmClicked;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_0;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_1;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_2;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_3;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_4;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_5;	
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CanFavouriteCreaftingCount;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CanCreaftingItemCount;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CanCreaftingCommonCount;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CanCreaftingToolCount;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CanCreaftingWeaponCount;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CanCreaftingTurretCount;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CanCreaftingStructuresCount;

	
};
