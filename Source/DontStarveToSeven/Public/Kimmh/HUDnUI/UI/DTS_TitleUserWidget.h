// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kimmh/HUDnUI/UI/DTS_UserWidget.h"
#include "DTS_TitleUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class DONTSTARVETOSEVEN_API UDTS_TitleUserWidget : public UDTS_UserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

protected:
	UFUNCTION()
	void OnNewGameButtonClick();

	UFUNCTION()
	void OnNewGameButtonHovered();

	UFUNCTION()
	void OnNewGameButtonUnHovered();
	
	UFUNCTION()
	void OnGameStartButtonClick();

	UFUNCTION()
	void OnGameStartButtonHovered();

	UFUNCTION()
	void OnGameStartButtonUnHovered();
	
	UFUNCTION()
	void OnWithFriendButtonClick();

	UFUNCTION()
	void OnWithFriendButtonHovered();

	UFUNCTION()
	void OnWithFriendButtonUnHovered();
	
	UFUNCTION()
	void OnSettingButtonClick();

	UFUNCTION()
	void OnSettingButtonHovered();

	UFUNCTION()
	void OnSettingButtonUnHovered();
	
	UFUNCTION()
	void OnExitButtonClick();

	UFUNCTION()
	void OnExitButtonHovered();

	UFUNCTION()
	void OnExitButtonUnHovered();

	bool IsNicknameViolation(const FText& NicknameText);

	bool IsServerReachable(const FString& AddressWithPort);

	void ShowNicknameViolationPopUp();

	void ShowServerReachablePopUp();

public:
	UPROPERTY(meta = (BindWidget))
	class UButton* NewGameButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* GameStartButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* WithFriendButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* SettingButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ExitButton;

	UPROPERTY(meta = (BindWidget))
	class UScaleBox* NewGameButtonScaleBox;

	UPROPERTY(meta = (BindWidget))
	class UScaleBox* GameStartButtonScaleBox;

	UPROPERTY(meta = (BindWidget))
	class UScaleBox* WithFriendButtonScaleBox;

	UPROPERTY(meta = (BindWidget))
	class UScaleBox* ScaleBoxSettingButton;

	UPROPERTY(meta = (BindWidget))
	class UScaleBox* ExitButtonScaleBox;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* NickNameEditableTextBox;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* IPAddressEditableTextBox;

	UPROPERTY(meta = (BindWidget))
	class UDTS_BasePopUpUserWidget* WBP_BasePopUp;

	UPROPERTY(meta = (BindWidget))
	class UDTS_ExitGameUpUserWidget* WBP_PopUpExitGame;

};
