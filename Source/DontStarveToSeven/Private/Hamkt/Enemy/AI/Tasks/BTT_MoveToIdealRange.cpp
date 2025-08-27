//// Fill out your copyright notice in the Description page of Project Settings.
//

#include "Hamkt/Enemy/AI/Tasks/BTT_MoveToIdealRange.h"
#include "AIController.h"                      // AIController 사용
#include "BehaviorTree/BlackboardComponent.h"   // UBlackboardComponent 사용
#include "GameFramework/Actor.h"               // AActor
#include "GameFramework/Pawn.h"                // APawn

UBTT_MoveToIdealRange::UBTT_MoveToIdealRange()
{
    NodeName = TEXT("Move To Ideal Range");
}

EBTNodeResult::Type UBTT_MoveToIdealRange::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // 1) AIController, Blackboard 참조 가져오기
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!IsValid(AIController)) return EBTNodeResult::Failed;

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!IsValid(BlackboardComp)) return EBTNodeResult::Failed;

    // 2) 공격 대상(Actor) 블랙보드에서 꺼내기
    UObject* TargetObject = BlackboardComp->GetValueAsObject(AttackTargetKey.SelectedKeyName);
    AActor* AttackTarget = Cast<AActor>(TargetObject);
    if (!IsValid(AttackTarget)) return EBTNodeResult::Failed;

    // 3) 사거리(float) 블랙보드에서 꺼내기
    float IdealRange = BlackboardComp->GetValueAsFloat(IdealRangeKey.SelectedKeyName);

    // 4) 이동 요청
    //    블루프린트의 "AI MoveTo" 노드와 똑같이
    //    Target Actor = AttackTarget, AcceptanceRadius = IdealRange
    //    여기서는 멀티/데디케이트 서버 상황에서도 동일한 로직
    //    (단순히 MoveToActor 호출)
    AIController->MoveToActor(AttackTarget, IdealRange);

     // 5) 즉시 Finish (블루프린트의 FinishExecute(Success=true))
    return EBTNodeResult::Succeeded;
}