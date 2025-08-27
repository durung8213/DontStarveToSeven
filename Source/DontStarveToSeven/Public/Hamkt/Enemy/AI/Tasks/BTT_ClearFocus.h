// Fill out your copyright notice in the Description page of Project Settings.

// 에너미가 횡으로 걷는 것을 방지하기 위한 트리 타스크
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

	// Task가 실행 될 떄 호출
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
protected:

private:
	
};
