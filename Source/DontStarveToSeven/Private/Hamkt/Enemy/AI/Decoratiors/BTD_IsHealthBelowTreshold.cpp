// Fill out your copyright notice in the Description page of Project Settings.


#include "Hamkt/Enemy/AI/Decoratiors/BTD_IsHealthBelowTreshold.h"
#include "AIController.h"
#include "Hamkt/Enemy/AI/BPI_EnemyAI.h"

UBTD_IsHealthBelowTreshold::UBTD_IsHealthBelowTreshold()
{
    NodeName = TEXT("BTD_IsHealthBelowThreshold");
    
}

FString UBTD_IsHealthBelowTreshold::GetStaticDescription() const
{
    return FString::Printf(TEXT("%s: %.2f"), *Super::GetStaticDescription(), HealthThreshold);;
}

// 현재 HP 상태를 체크. 0보다 낮으면 BT를 통해 죽음판정
bool UBTD_IsHealthBelowTreshold::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{

    // AI 컨트롤러 가져오기
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!IsValid(AIController)) return false;

    // AI의 Pawn 가져오기
    APawn* AIPawn = AIController->GetPawn();
    if (!IsValid(AIPawn)) return false;

    // HP에 관련된 인터페이스 함수들을 가지고 와서 현재 값을 Check
    IBPI_EnemyAI* EnemyAI = Cast<IBPI_EnemyAI>(AIPawn);
    if (!EnemyAI)
    {
        return false;
    }

    float MaxHealth = IBPI_EnemyAI::Execute_GetMaxHealth(AIPawn);
    float CurrentHealth = IBPI_EnemyAI::Execute_GetCurrentHealth(AIPawn);

    float result = CurrentHealth / MaxHealth;
    
    if (result <= HealthThreshold)
    {
        return true;
    }
    else
    {
        return false;
    }
}
