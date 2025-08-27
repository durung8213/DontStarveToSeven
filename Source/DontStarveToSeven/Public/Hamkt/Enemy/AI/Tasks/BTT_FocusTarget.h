// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_FocusTarget.generated.h"

/**
 * 
 */
UCLASS()
class DONTSTARVETOSEVEN_API UBTT_FocusTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:
    // 생성자
    UBTT_FocusTarget();

    // 실행 함수 (BTTaskNode)
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
   

private:
    /** Blackboard에 저장할 타깃 키 */
    UPROPERTY(EditAnywhere, Category = "Keys")
    FBlackboardKeySelector TargetKey;
};