// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Hamkt/Enemy/AI/Enums/E_MovementSpeed.h"
#include "BTT_SetMovementSpeed.generated.h"
/**
 * 
 */

UCLASS()
class DONTSTARVETOSEVEN_API UBTT_SetMovementSpeed : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_SetMovementSpeed();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, UINT8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed")
	E_MovementSpeed MovementSpeed;	// 비헤이비어트리에서 값이 설정됨
};
