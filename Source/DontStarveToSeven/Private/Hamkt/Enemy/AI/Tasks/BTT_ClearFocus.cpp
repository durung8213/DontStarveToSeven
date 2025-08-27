
// 에너미가 횡으로 걷는 것을 방지하기 위한 트리 타스크

#include "Hamkt/Enemy/AI/Tasks/BTT_ClearFocus.h"
#include "AIController.h"

UBTT_ClearFocus::UBTT_ClearFocus()
{
    NodeName = TEXT("BTT_ClearFocus");
    
}

EBTNodeResult::Type UBTT_ClearFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // OwnerComp로부터 AI Controller 가져옴
    AAIController* AIController = OwnerComp.GetAIOwner();
        // 가져오지 못할경우 Fail
    if (!AIController)
    {
        return EBTNodeResult::Failed;
    }

    // AI Controller의 Focus 해제
    AIController->ClearFocus(EAIFocusPriority::Default);

    return EBTNodeResult::Succeeded;
}
