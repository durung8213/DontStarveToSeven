// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kimmh/Character/Controller/DTS_GamePlayPlayerController.h"
#include "TestController.generated.h"

/**
 * 
 */
UCLASS()
class DONTSTARVETOSEVEN_API ATestController : public ADTS_GamePlayPlayerController
{
	GENERATED_BODY()
	
	virtual void CreateStructure() override;
	
	virtual void CancleCraftingMode() override;
};
