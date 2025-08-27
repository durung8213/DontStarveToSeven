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

// 이상적인 범위 구하기
bool UBTD_IsWithinIdealRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    // AI 컨트롤러 가져오기
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!IsValid(AIController)) return false;

    // AIC_EnemyBase로 캐스팅
    AAIC_EnemyBase* EnemyAI = Cast<AAIC_EnemyBase>(AIController);
    if (!IsValid(EnemyAI)) return false;

    // OwnerComp에서 블랙보드 컴포넌트를 가져옴
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!IsValid(BlackboardComp)) return false;

    // 블랙보드에서 공격 대상(Actor) 가져오기
    AActor* AttackTarget = Cast<AActor>(BlackboardComp->GetValueAsObject(AttackTargetKey.SelectedKeyName));
    if (!IsValid(AttackTarget)) return false;

    // 블랙보드에서 IdealRange 값을 가져오기
    float IdealRangeValue = BlackboardComp->GetValueAsFloat(IdealRangeKey.SelectedKeyName);
  
    // AI의 Pawn 가져오기
    APawn* AIPawn = AIController->GetPawn();
    if (!IsValid(AIPawn)) return false;

    // AI와 공격 대상 사이의 거리 계산
    float Distance = AIPawn->GetDistanceTo(AttackTarget);

    // 오차거리
    const float ErrorMargin = 50.f;

    // 거리 - 오차 범위가 IdealRange 이하인가? 해당 범위 안에 있는가?
    return (Distance - ErrorMargin) <= IdealRangeValue;
}
