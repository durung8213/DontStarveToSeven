// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Kimmh/Enum/InteractionEnum.h"
#include "DTS_BPIInteraction.generated.h"
/**
 *
 */

UINTERFACE(Blueprintable)
class DONTSTARVETOSEVEN_API UDTS_BPIInteraction : public UInterface
{
	GENERATED_BODY()
};


/**
 * IBPI_EnemyAI
 * - 실제 인터페이스 함수들을 선언하는 순수 C++ 인터페이스 클래스입니다.
 *   이 인터페이스는 Blueprint와 C++ 모두에서 구현할 수 있습니다.
 */
class DONTSTARVETOSEVEN_API IDTS_BPIInteraction
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
	UDTS_BaseInteractionUserWidget* GetBaseInteractionWidget();
	/**
	 * Blueprint에서 구현 가능한 함수 예시.
	 * BlueprintNativeEvent를 사용하면 기본 C++ 구현을 제공할 수 있으며, Blueprint에서 오버라이드할 수도 있습니다.
	 */

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
	void Interaction(ADTS_GamePlayPlayerController* InteractionPlayerController, ABasePlayerCharacter* InteractionPlayerCharacter);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
	EInteraction GetInteractionType();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
	int GetInteractionHp();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
	bool GetCanInteraction();
};