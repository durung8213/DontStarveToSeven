// Fill out your copyright notice in the Description page of Project Settings.

// ���ʹ̰� Ⱦ���� �ȴ� ���� �����ϱ� ���� Ʈ�� Ÿ��ũ
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_ClearFocus.generated.h"

/**
 * 
 */
UCLASS()
class DONTSTARVETOSEVEN_API UBTT_ClearFocus : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_ClearFocus();

	// Task�� ���� �� �� ȣ��
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
protected:

private:
	
};
