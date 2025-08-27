// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kimmh/HUDnUI/UI/DTS_UserWidget.h"
#include "DTS_BasePopUpUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class DONTSTARVETOSEVEN_API UDTS_BasePopUpUserWidget : public UDTS_UserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnEmptySpaceButtonClick();

	UFUNCTION()
	virtual void OnOkButtonClick();

	void SetPopUpMessage(const FText& Message);

public:
	//FText::FromString(TEXT("Hello World"))

	UPROPERTY(meta = (BindWidget))
	class UButton* EmptySpaceButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* OkButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MessageTextBlock;


};
