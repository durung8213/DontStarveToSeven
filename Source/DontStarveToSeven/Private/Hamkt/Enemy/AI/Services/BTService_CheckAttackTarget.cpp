// Fill out your copyright notice in the Description page of Project Settings.


#include "Hamkt/Enemy/AI/Services/BTService_CheckAttackTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Hamkt/Enemy/AI/AIC_EnemyBase.h"
#include "Hamkt/Enemy/AI/Enums/E_AIState.h"
#include "GameFramework/Pawn.h"

UBTService_CheckAttackTarget::UBTService_CheckAttackTarget()
{
    NodeName = TEXT("Check AttackTarget Validity");
    // Interval를 0.5초 정도로 설정할 수 있습니다.
    Interval = 0.5f;
    CheckInterval = 0.5f;
}

void UBTService_CheckAttackTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        return;
    }

    // Blackboard에 저장된 AttackTarget 키의 값을 가져옴
    UObject* TargetObject = BlackboardComp->GetValueAsObject(FName("AttackTarget"));

    // 만약 AttackTarget이 설정되지 않았거나 유효하지 않다면,
    if (!IsValid(TargetObject))
    {
        // 상태를 Passive로 전환하고 AttackTarget 키를 클리어합니다.
        BlackboardComp->SetValueAsEnum(FName("State"), static_cast<uint8>(E_AIState::Passive));
        BlackboardComp->ClearValue(FName("AttackTarget"));
        UE_LOG(LogTemp, Log, TEXT("[BTService] AttackTarget is invalid, clearing key and setting state to Passive."));
    }
}
