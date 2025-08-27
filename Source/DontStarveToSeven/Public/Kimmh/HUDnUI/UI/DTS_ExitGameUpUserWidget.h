// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kimmh/HUDnUI/UI/DTS_BasePopUpUserWidget.h"
#include "DTS_ExitGameUpUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class DONTSTARVETOSEVEN_API UDTS_ExitGameUpUserWidget : public UDTS_BasePopUpUserWidget
{
	GENERATED_BODY()
	virtual void NativeConstruct() override;
	
public:
	virtual void OnOkButtonClick() override;
	
	UFUNCTION()
	void OnNoButtonClick();

public:
	UPROPERTY(meta = (BindWidget))
	class UButton* NoButton;
};
