//// Fill out your copyright notice in the Description page of Project Settings.
//

#include "Hamkt/Enemy/AI/Tasks/BTT_MoveToIdealRange.h"
#include "AIController.h"                      // AIController ���
#include "BehaviorTree/BlackboardComponent.h"   // UBlackboardComponent ���
#include "GameFramework/Actor.h"               // AActor
#include "GameFramework/Pawn.h"                // APawn

UBTT_MoveToIdealRange::UBTT_MoveToIdealRange()
{
    NodeName = TEXT("Move To Ideal Range");
}

EBTNodeResult::Type UBTT_MoveToIdealRange::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // 1) AIController, Blackboard ���� ��������
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!IsValid(AIController)) return EBTNodeResult::Failed;

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!IsValid(BlackboardComp)) return EBTNodeResult::Failed;

    // 2) ���� ���(Actor) �����忡�� ������
    UObject* TargetObject = BlackboardComp->GetValueAsObject(AttackTargetKey.SelectedKeyName);
    AActor* AttackTarget = Cast<AActor>(TargetObject);
    if (!IsValid(AttackTarget)) return EBTNodeResult::Failed;

    // 3) ��Ÿ�(float) �����忡�� ������
    float IdealRange = BlackboardComp->GetValueAsFloat(IdealRangeKey.SelectedKeyName);

    // 4) �̵� ��û
    //    �������Ʈ�� "AI MoveTo" ���� �Ȱ���
    //    Target Actor = AttackTarget, AcceptanceRadius = IdealRange
    //    ���⼭�� ��Ƽ/��������Ʈ ���� ��Ȳ������ ������ ����
    //    (�ܼ��� MoveToActor ȣ��)
    AIController->MoveToActor(AttackTarget, IdealRange);

     // 5) ��� Finish (�������Ʈ�� FinishExecute(Success=true))
    return EBTNodeResult::Succeeded;
}