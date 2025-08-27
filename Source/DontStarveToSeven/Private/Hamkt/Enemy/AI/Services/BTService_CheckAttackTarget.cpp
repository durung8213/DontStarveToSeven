// Fill out your copyright notice in the Description page of Project Settings.


#include "Hamkt/Enemy/AI/Services/BTService_CheckAttackTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Hamkt/Enemy/AI/AIC_EnemyBase.h"
#include "Hamkt/Enemy/AI/Enums/E_AIState.h"
#include "GameFramework/Pawn.h"

UBTService_CheckAttackTarget::UBTService_CheckAttackTarget()
{
    NodeName = TEXT("Check AttackTarget Validity");
    // Interval�� 0.5�� ������ ������ �� �ֽ��ϴ�.
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

    // Blackboard�� ����� AttackTarget Ű�� ���� ������
    UObject* TargetObject = BlackboardComp->GetValueAsObject(FName("AttackTarget"));

    // ���� AttackTarget�� �������� �ʾҰų� ��ȿ���� �ʴٸ�,
    if (!IsValid(TargetObject))
    {
        // ���¸� Passive�� ��ȯ�ϰ� AttackTarget Ű�� Ŭ�����մϴ�.
        BlackboardComp->SetValueAsEnum(FName("State"), static_cast<uint8>(E_AIState::Passive));
        BlackboardComp->ClearValue(FName("AttackTarget"));
        UE_LOG(LogTemp, Log, TEXT("[BTService] AttackTarget is invalid, clearing key and setting state to Passive."));
    }
}
