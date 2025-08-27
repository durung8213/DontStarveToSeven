// Fill out your copyright notice in the Description page of Project Settings.


#include "Hamkt/Enemy/AI/Tasks/BTT_DefaultAttack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Hamkt/Enemy/EnemyBase.h"
#include "GameFramework/Pawn.h"

UBTT_DefaultAttack::UBTT_DefaultAttack()
{
    // 노드 이름을 Default Attack으로 설정
    NodeName = TEXT("BTT_Default Attack");

    CachedOwnerComp = nullptr;

    bCreateNodeInstance = true;
    
}

void UBTT_DefaultAttack::FinishedAttacking()
{
    // FinishedAttacking 커스텀 이벤트가 호출되면, 먼저 바인딩 해제
    if (CachedOwnerComp)
    {
        AAIController* AIController = CachedOwnerComp->GetAIOwner();
        if (AIController)
        {
            AEnemyBase* EnemyPawn = Cast<AEnemyBase>(AIController->GetPawn());
            if (EnemyPawn)
            {
                EnemyPawn->OnAttackEnd.RemoveDynamic(this, &UBTT_DefaultAttack::FinishedAttacking);
            }
        }
        
        FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
    }
}

EBTNodeResult::Type UBTT_DefaultAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, UINT8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (AIController)
    {
        APawn* AIPawn = AIController->GetPawn();
        if (AIPawn)
        {
            // AEnemyBase 타입으로 캐스팅
            AEnemyBase* EnemyPawn = Cast<AEnemyBase>(AIPawn);
            if (EnemyPawn)
            {
                // Behavior Tree Task 완료를 위해 OwnerComp 포인터를 캐시
                CachedOwnerComp = &OwnerComp;

                // 공격 함수 호출
                EnemyPawn->Attack(FName("Attack"));

                FString Message = FString::Printf(TEXT("Attack"));
                GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, Message);

                // OnAttackEnd 이벤트에 FinishedAttacking 이벤트 핸들러 바인딩
                EnemyPawn->OnAttackEnd.RemoveDynamic(this, &UBTT_DefaultAttack::FinishedAttacking);
                EnemyPawn->OnAttackEnd.AddDynamic(this, &UBTT_DefaultAttack::FinishedAttacking);
                
                // 공격이 진행 중이므로 Task는 InProgress 상태...
                return EBTNodeResult::InProgress;
            }
        }
    }
    return EBTNodeResult::Failed;
}
