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

// 실행되면 패시브 상태로 전환
EBTNodeResult::Type UBTT_SetStateAsPassive::ExecuteTask(UBehaviorTreeComponent& OwnerComp, UINT8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();

    if (AIController)
    {
        AAIC_EnemyBase* EnemyBase = Cast<AAIC_EnemyBase>(AIController);     // 현재 AIController를 AAIC_EnemyBase 클래스로 형변환
        
        if (EnemyBase)
        {
            EnemyBase->SetStateAsPassive();     // 블랙보드키를 패시브 상태로 전환
            return EBTNodeResult::Succeeded;    
        }
    }

    return EBTNodeResult::Failed;
}
