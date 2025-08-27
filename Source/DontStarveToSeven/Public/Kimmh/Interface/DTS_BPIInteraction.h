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
 * - ���� �������̽� �Լ����� �����ϴ� ���� C++ �������̽� Ŭ�����Դϴ�.
 *   �� �������̽��� Blueprint�� C++ ��ο��� ������ �� �ֽ��ϴ�.
 */
class DONTSTARVETOSEVEN_API IDTS_BPIInteraction
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
	UDTS_BaseInteractionUserWidget* GetBaseInteractionWidget();
	/**
	 * Blueprint���� ���� ������ �Լ� ����.
	 * BlueprintNativeEvent�� ����ϸ� �⺻ C++ ������ ������ �� ������, Blueprint���� �������̵��� ���� �ֽ��ϴ�.
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