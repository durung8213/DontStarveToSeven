// Fill out your copyright notice in the Description page of Project Settings.


#include "Hamkt/Enemy/AI/Tasks/BTT_DefaultAttack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Hamkt/Enemy/EnemyBase.h"
#include "GameFramework/Pawn.h"

UBTT_DefaultAttack::UBTT_DefaultAttack()
{
    // ��� �̸��� Default Attack���� ����
    NodeName = TEXT("BTT_Default Attack");

    CachedOwnerComp = nullptr;

    bCreateNodeInstance = true;
    
}

void UBTT_DefaultAttack::FinishedAttacking()
{
    // FinishedAttacking Ŀ���� �̺�Ʈ�� ȣ��Ǹ�, ���� ���ε� ����
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
            // AEnemyBase Ÿ������ ĳ����
            AEnemyBase* EnemyPawn = Cast<AEnemyBase>(AIPawn);
            if (EnemyPawn)
            {
                // Behavior Tree Task �ϷḦ ���� OwnerComp �����͸� ĳ��
                CachedOwnerComp = &OwnerComp;

                // ���� �Լ� ȣ��
                EnemyPawn->Attack(FName("Attack"));

                FString Message = FString::Printf(TEXT("Attack"));
                GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, Message);

                // OnAttackEnd �̺�Ʈ�� FinishedAttacking �̺�Ʈ �ڵ鷯 ���ε�
                EnemyPawn->OnAttackEnd.RemoveDynamic(this, &UBTT_DefaultAttack::FinishedAttacking);
                EnemyPawn->OnAttackEnd.AddDynamic(this, &UBTT_DefaultAttack::FinishedAttacking);
                
                // ������ ���� ���̹Ƿ� Task�� InProgress ����...
                return EBTNodeResult::InProgress;
            }
        }
    }
    return EBTNodeResult::Failed;
}
