// Fill out your copyright notice in the Description page of Project Settings.


#include "Hamkt/Enemy/AI/Tasks/BTT_SetMovementSpeed.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Hamkt/Enemy/EnemyBase.h"
#include "Hamkt/Enemy/AI/BPI_EnemyAI.h"
#include "GameFramework/Pawn.h"


UBTT_SetMovementSpeed::UBTT_SetMovementSpeed()
{
    NodeName = TEXT("BTT_SetMovementSpeed");
}

EBTNodeResult::Type UBTT_SetMovementSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, UINT8* NodeMemory)
{
    // Behavior Tree Task�� ���� ���� ��, OwnerComp(UBehaviorTreeComponent)�� GetAIOwner() �Լ��� ����� 
        // ���� ���� ���� AIController�� ��ȯ
    AAIController* AIController = OwnerComp.GetAIOwner();

    // nullptr ���¶��?
    if (!AIController)
    {
        return EBTNodeResult::Failed;
    }

    // AIController�� ������ �����ϰ� �ִ� Pawn�� �����ɴϴ�.
        // Pawn�� ���� AI�� �����ϴ� ĳ����(��: AEnemyBase, ACharacter)
    APawn* AIPawn = AIController->GetPawn();

    // nullptr ���¶��?
    if (!AIPawn)
    {
        return EBTNodeResult::Failed;
    }

    // ��� A) ImplementsInterface + Execute_�Լ��� ȣ��
    if (AIPawn->GetClass()->ImplementsInterface(UBPI_EnemyAI::StaticClass()))
    {
        // ���ϴ� ������ ���¸� ����. �����̺��Ʈ������ MovementSpeed�� ���� �޾ƿ�
        IBPI_EnemyAI::Execute_SetMovementSpeed(AIPawn, MovementSpeed);

        return EBTNodeResult::Succeeded;
    }
     
   // ��� B) ���� C++ ĳ�����Ͽ� �������̽� �Լ� ���
   // (����: BPI_EnemyAI�� C++���� �����߰�, AEnemyBase���� �ش� �������̽� ���/����)
    
  /* 
  IBPI_EnemyAI* EnemyInterface = Cast<IBPI_EnemyAI>(AIPawn);
   if (EnemyInterface)
   {
       EnemyInterface->SetMovementSpeed(E_MovementSpeed::Jogging);
       return EBTNodeResult::Succeeded;
  } 
  */
   
    return EBTNodeResult::Failed;
}
