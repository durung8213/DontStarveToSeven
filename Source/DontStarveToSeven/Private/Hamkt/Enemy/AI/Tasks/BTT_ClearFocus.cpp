
// ���ʹ̰� Ⱦ���� �ȴ� ���� �����ϱ� ���� Ʈ�� Ÿ��ũ

#include "Hamkt/Enemy/AI/Tasks/BTT_ClearFocus.h"
#include "AIController.h"

UBTT_ClearFocus::UBTT_ClearFocus()
{
    NodeName = TEXT("BTT_ClearFocus");
    
}

EBTNodeResult::Type UBTT_ClearFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // OwnerComp�κ��� AI Controller ������
    AAIController* AIController = OwnerComp.GetAIOwner();
        // �������� ���Ұ�� Fail
    if (!AIController)
    {
        return EBTNodeResult::Failed;
    }

    // AI Controller�� Focus ����
    AIController->ClearFocus(EAIFocusPriority::Default);

    return EBTNodeResult::Succeeded;
}
