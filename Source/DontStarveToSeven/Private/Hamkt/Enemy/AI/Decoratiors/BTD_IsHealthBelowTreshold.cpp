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

// ���� HP ���¸� üũ. 0���� ������ BT�� ���� ��������
bool UBTD_IsHealthBelowTreshold::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{

    // AI ��Ʈ�ѷ� ��������
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!IsValid(AIController)) return false;

    // AI�� Pawn ��������
    APawn* AIPawn = AIController->GetPawn();
    if (!IsValid(AIPawn)) return false;

    // HP�� ���õ� �������̽� �Լ����� ������ �ͼ� ���� ���� Check
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
