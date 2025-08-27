// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "BossStatusWidget.generated.h"

/**
 * 
 */
UCLASS()
class DONTSTARVETOSEVEN_API UBossStatusWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void UpdateBossName(FString BossName);
	void UpdateBossHPBar(float CurrentHP, float MaxHP);

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock_BossName;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* ProgressBar_HPBar;
};
