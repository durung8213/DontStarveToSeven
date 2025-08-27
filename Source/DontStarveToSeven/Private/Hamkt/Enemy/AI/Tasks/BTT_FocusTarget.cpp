// Fill out your copyright notice in the Description page of Project Settings.

// 타겟을 지정

#include "Hamkt/Enemy/AI/Tasks/BTT_FocusTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTT_FocusTarget::UBTT_FocusTarget()
{
    NodeName = TEXT("BTT_Focus Target");  // BT Task 노드 이름
    
}

EBTNodeResult::Type UBTT_FocusTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // 1) AIController, Blackboard 컴포넌트
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        return EBTNodeResult::Failed;
    }

    UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
    if (!BBComp)
    {
        return EBTNodeResult::Failed;
    }

    AActor* AttackTarget = Cast<AActor>(BBComp->GetValueAsObject(TargetKey.SelectedKeyName));
    if (AttackTarget)
    {
        AIController->SetFocus(AttackTarget);
        
        return EBTNodeResult::Succeeded;
    }


    return EBTNodeResult::Failed;
}