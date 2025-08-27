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
    // Behavior Tree Task가 실행 중일 때, OwnerComp(UBehaviorTreeComponent)의 GetAIOwner() 함수를 사용해 
        // 현재 동작 중인 AIController를 반환
    AAIController* AIController = OwnerComp.GetAIOwner();

    // nullptr 상태라면?
    if (!AIController)
    {
        return EBTNodeResult::Failed;
    }

    // AIController가 실제로 제어하고 있는 Pawn을 가져옵니다.
        // Pawn은 보통 AI가 조종하는 캐릭터(예: AEnemyBase, ACharacter)
    APawn* AIPawn = AIController->GetPawn();

    // nullptr 상태라면?
    if (!AIPawn)
    {
        return EBTNodeResult::Failed;
    }

    // 방법 A) ImplementsInterface + Execute_함수로 호출
    if (AIPawn->GetClass()->ImplementsInterface(UBPI_EnemyAI::StaticClass()))
    {
        // 원하는 열거형 상태를 지정. 비헤이비어트리에서 MovementSpeed의 값을 받아옴
        IBPI_EnemyAI::Execute_SetMovementSpeed(AIPawn, MovementSpeed);

        return EBTNodeResult::Succeeded;
    }
     
   // 방법 B) 직접 C++ 캐스팅하여 인터페이스 함수 사용
   // (전제: BPI_EnemyAI를 C++에서 선언했고, AEnemyBase에서 해당 인터페이스 상속/구현)
    
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
