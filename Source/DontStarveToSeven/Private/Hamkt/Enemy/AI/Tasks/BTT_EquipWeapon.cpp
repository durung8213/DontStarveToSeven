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
    if (!IsValid(AIController)) return EBTNodeResult::Failed;   // 가져오지 못했을 경우 Failed 반환

    // Blackboard Component를 가져옵니다.
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!IsValid(BlackboardComp)) return EBTNodeResult::Failed;   // 가져오지 못했을 경우 Failed 반환

    // AI의 Pawn 가져오기
    APawn* AIPawn = AIController->GetPawn();
    if (!IsValid(AIPawn)) return EBTNodeResult::Failed;   // 가져오지 못했을 경우 Failed 반환

    // EnemyBase 가져오기
    AEnemyBase* EnemyBase = Cast<AEnemyBase>(AIPawn);
    if (!IsValid(EnemyBase)) return EBTNodeResult::Failed;   // 가져오지 못했을 경우 Failed 반환

    //EnemyBase->EquipWeapon(); 수정



    

    return EBTNodeResult::Type();
}
