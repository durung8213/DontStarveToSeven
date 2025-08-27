// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kimmh/HUDnUI/UI/DTS_UserWidget.h"
#include "DTS_UserStateUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class DONTSTARVETOSEVEN_API UDTS_UserStateUserWidget : public UDTS_UserWidget
{
	GENERATED_BODY()

public:

	/// <summary>
	/// 하는거 없음
	/// </summary>
	virtual void NativeConstruct() override;
	
	
	void Init();

	void SetMaxHp(float CurrentHp);
	void SetCurrentHp(float CurrentHp);
	void SetMaxMp(float MaxMp);
	void SetCurrentMp(float CurrentMp);
	void SetMaxHunger(float MaxHunger);
	void SetCurrentHunger(float CurrentHunger);

	void SelectQuickSlot(int SlotNum);
	
	void ReLoadQuickSlot();

	void SetBossHpBarVisibility(bool bIsVisbile);

	void SetBossHpBarPercent(float InCurrentHealth, float InMaxHealth);

	void SetBossName(FText BossName);

public:
	UPROPERTY(meta = (BindWidget))
	class UDTS_StateSlotUserWidget* WBP_HpStateSlot;

	UPROPERTY(meta = (BindWidget))
	class UDTS_StateSlotUserWidget* WBP_MpStateSlot;

	UPROPERTY(meta = (BindWidget))
	class UDTS_StateSlotUserWidget* WBP_HungerStateSlot;
	
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, EditAnywhere)
	class UDTS_QuickSlotUserWidget* WBP_QuickSlot;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, EditAnywhere)
	class UDTS_BossHpBarUserWidget* WBP_BossHpBar;

	FTimerHandle InitHandle;
};
