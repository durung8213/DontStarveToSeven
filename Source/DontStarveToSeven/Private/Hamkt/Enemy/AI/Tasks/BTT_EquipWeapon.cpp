// Fill out your copyright notice in the Description page of Project Settings.


#include "Hamkt/Enemy/AI/Tasks/BTT_EquipWeapon.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Hamkt/Enemy/EnemyBase.h"
#include "Hamkt/Enemy/AI/BPI_EnemyAI.h"

UBTT_EquipWeapon::UBTT_EquipWeapon()
{
    NodeName = TEXT("BTT_EquipWeapon");
}

EBTNodeResult::Type UBTT_EquipWeapon::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!IsValid(AIController)) return EBTNodeResult::Failed;   // �������� ������ ��� Failed ��ȯ

    // Blackboard Component�� �����ɴϴ�.
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!IsValid(BlackboardComp)) return EBTNodeResult::Failed;   // �������� ������ ��� Failed ��ȯ

    // AI�� Pawn ��������
    APawn* AIPawn = AIController->GetPawn();
    if (!IsValid(AIPawn)) return EBTNodeResult::Failed;   // �������� ������ ��� Failed ��ȯ

    // EnemyBase ��������
    AEnemyBase* EnemyBase = Cast<AEnemyBase>(AIPawn);
    if (!IsValid(EnemyBase)) return EBTNodeResult::Failed;   // �������� ������ ��� Failed ��ȯ

    //EnemyBase->EquipWeapon(); ����



    

    return EBTNodeResult::Type();
}
