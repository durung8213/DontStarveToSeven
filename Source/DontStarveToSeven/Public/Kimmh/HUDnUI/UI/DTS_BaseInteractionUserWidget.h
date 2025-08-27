// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kimmh/HUDnUI/UI/DTS_UserWidget.h"
#include "DTS_BaseInteractionUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class DONTSTARVETOSEVEN_API UDTS_BaseInteractionUserWidget : public UDTS_UserWidget
{
	GENERATED_BODY()

public:
	/// <summary>
	/// 하는거 없음
	/// </summary>
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

	UFUNCTION()
	void SetVisibilityWidget();

	UFUNCTION()
	void SetCollapsedWidget();

	void SetInteractionText(FText InteractionText);

	void SetInputKeyVisible();

	void SetInputKeyCollapsed();

	void SetRoundPorgressBarVisible(float Duraction);
	
	void SetRoundProgressBarCollapsed();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetPercent(float Percent);

	/// <summary>
	/// 프로그래스 바가 끝나면 발생할 이벤트
	/// </summary>
	virtual void ProgressComplete();
	
public:
	UPROPERTY(BlueprintReadWrite ,meta = (BindWidget))
	class UImage* RoundProgressBar;

	UPROPERTY(meta = (BindWidget))
	class UImage* InputKey;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* InteractionTextBlock;

	float DurationTime = 0;

	float ElapsedTime = 0;

	bool bProgressBarActive = false;

	bool bIsProgressComplete = false;

	FTimerHandle VisableTimer;
};
