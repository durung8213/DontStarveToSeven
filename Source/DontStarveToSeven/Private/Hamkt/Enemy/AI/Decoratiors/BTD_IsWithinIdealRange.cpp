// Fill out your copyright notice in the Description page of Project Settings.


#include "Hamkt/Enemy/AI/Decoratiors/BTD_IsWithinIdealRange.h"
#include "AIController.h"
#include "Hamkt/Enemy/AI/AIC_EnemyBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Actor.h"

UBTD_IsWithinIdealRange::UBTD_IsWithinIdealRange()
{
    NodeName = TEXT("BTD_IsWithinIdealRange");
}

// �̻����� ���� ���ϱ�
bool UBTD_IsWithinIdealRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    // AI ��Ʈ�ѷ� ��������
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!IsValid(AIController)) return false;

    // AIC_EnemyBase�� ĳ����
    AAIC_EnemyBase* EnemyAI = Cast<AAIC_EnemyBase>(AIController);
    if (!IsValid(EnemyAI)) return false;

    // OwnerComp���� ������ ������Ʈ�� ������
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!IsValid(BlackboardComp)) return false;

    // �����忡�� ���� ���(Actor) ��������
    AActor* AttackTarget = Cast<AActor>(BlackboardComp->GetValueAsObject(AttackTargetKey.SelectedKeyName));
    if (!IsValid(AttackTarget)) return false;

    // �����忡�� IdealRange ���� ��������
    float IdealRangeValue = BlackboardComp->GetValueAsFloat(IdealRangeKey.SelectedKeyName);
  
    // AI�� Pawn ��������
    APawn* AIPawn = AIController->GetPawn();
    if (!IsValid(AIPawn)) return false;

    // AI�� ���� ��� ������ �Ÿ� ���
    float Distance = AIPawn->GetDistanceTo(AttackTarget);

    // �����Ÿ�
    const float ErrorMargin = 50.f;

    // �Ÿ� - ���� ������ IdealRange �����ΰ�? �ش� ���� �ȿ� �ִ°�?
    return (Distance - ErrorMargin) <= IdealRangeValue;
}
