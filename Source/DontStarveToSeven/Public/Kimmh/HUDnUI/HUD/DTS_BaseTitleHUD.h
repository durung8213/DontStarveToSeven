// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kimmh/HUDnUI/HUD/DTS_BaseHUD.h"
#include "DTS_BaseTitleHUD.generated.h"

/**
 * 
 */
UCLASS()
class DONTSTARVETOSEVEN_API ADTS_BaseTitleHUD : public ADTS_BaseHUD
{
	GENERATED_BODY()
	
	ADTS_BaseTitleHUD();

	virtual void BeginPlay() override;

	class UDTS_TitleUserWidget* TitleUserWidget;

};
