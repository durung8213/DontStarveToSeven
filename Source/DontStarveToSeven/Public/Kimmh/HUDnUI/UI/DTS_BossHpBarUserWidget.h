// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kimmh/HUDnUI/UI/DTS_UserWidget.h"
#include "DTS_BossHpBarUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class DONTSTARVETOSEVEN_API UDTS_BossHpBarUserWidget : public UDTS_UserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void SetBossHpBarVisibility(bool bIsVisbile);

	void SetBossName(FText BossName);

	void SetBossName(FString BossName);

	void SetBossHpBarPercent(float InCurrentHealth, float InMaxHealth);

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* BossHpBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* BossNameTextBlock;
};
