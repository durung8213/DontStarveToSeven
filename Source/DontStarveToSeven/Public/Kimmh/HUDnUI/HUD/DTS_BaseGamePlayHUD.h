// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kimmh/HUDnUI/HUD/DTS_BaseHUD.h"
#include "DTS_BaseGamePlayHUD.generated.h"

/**
 * 
 */
UCLASS()
class DONTSTARVETOSEVEN_API ADTS_BaseGamePlayHUD : public ADTS_BaseHUD
{
	GENERATED_BODY()

	ADTS_BaseGamePlayHUD();

public:
	virtual void BeginPlay() override;

	void OpenPlayerMain();

	void ClosePlayerMain();

	void CreatePlayerMainWidget();

	void ReLoadInventory();

	void ReLoadQuickSlot();

	void OpenCreaftingUI();

	void CloseCreaftingUI();

	void CreateCreaftingUI();

	void SelectQuickSlot(int SlotNum);

	void BindBossTakeDamage(class AEnemyBossBase* Boss);

	void UnBindBossTakeDamage(class AEnemyBossBase* Boss);

	void SetBossHpBarVisibility(bool bIsVisbile);

	UFUNCTION()
	void SetBossHpBarPercent(float InCurrentHealth, float InMaxHealth);

	void SetBossName(FText BossName);

public:
	UPROPERTY()
	class UDTS_UserStateUserWidget* DTSUserStateUserWidget;


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UDTS_CreaftingUserWidget> DTS_CreaftingUserWidgetClass;


private:
	UPROPERTY()
	class UDTS_PlayerMainUserWidget* DTSPlayerMainUserWidget;

	UPROPERTY()
	class UDTS_CreaftingUserWidget* DTSCreaftingUserWidget;


};
