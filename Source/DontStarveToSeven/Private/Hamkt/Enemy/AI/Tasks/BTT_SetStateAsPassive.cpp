// Fill out your copyright notice in the Description page of Project Settings.


#include "Hamkt/Enemy/AI/Tasks/BTT_SetStateAsPassive.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Hamkt/Enemy/AI/AIC_EnemyBase.h"
#include "GameFramework/Pawn.h"


UBTT_SetStateAsPassive::UBTT_SetStateAsPassive()
{
    NodeName = TEXT("BTT_SetStateAsPassive");
}

// ����Ǹ� �нú� ���·� ��ȯ
EBTNodeResult::Type UBTT_SetStateAsPassive::ExecuteTask(UBehaviorTreeComponent& OwnerComp, UINT8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();

    if (AIController)
    {
        AAIC_EnemyBase* EnemyBase = Cast<AAIC_EnemyBase>(AIController);     // ���� AIController�� AAIC_EnemyBase Ŭ������ ����ȯ
        
        if (EnemyBase)
        {
            EnemyBase->SetStateAsPassive();     // ������Ű�� �нú� ���·� ��ȯ
            return EBTNodeResult::Succeeded;    
        }
    }

    return EBTNodeResult::Failed;
}
