// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Kimmh/HUDnUI/CustomWidget/DTS_CustomSButton.h"
#include "Templates/SharedPointer.h" 
#include "DTS_CustomButton.generated.h"

/**
 * 
 */
UCLASS()
class DONTSTARVETOSEVEN_API UDTS_CustomButton : public UButton
{
	GENERATED_BODY()
protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

protected:
	TSharedPtr<DTS_CustomSButton> MyButton;

public:

};
