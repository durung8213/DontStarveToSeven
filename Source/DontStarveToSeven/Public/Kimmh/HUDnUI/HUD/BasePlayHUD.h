// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BasePlayHUD.generated.h"

/**
 * 
 */
UCLASS()
class DONTSTARVETOSEVEN_API ABasePlayHUD : public AHUD
{
	GENERATED_BODY()
	

public:
	virtual void BeginPlay() override;

	class UDTS_PlayerMainUserWidget* CreatePlayerMainWidget;


public:

private:
	UPROPERTY()
	class UDTS_PlayerMainUserWidget* DTSPlayerMainUserWidget;

};