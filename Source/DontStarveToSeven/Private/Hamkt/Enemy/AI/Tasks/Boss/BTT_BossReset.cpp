// Fill out your copyright notice in the Description page of Project Settings.


#include "Hamkt/Enemy/AI/Tasks/Boss/BTT_BossReset.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"


UBTT_BossReset::UBTT_BossReset()
{
	NodeName = TEXT("BTT_BossReset");
}

EBTNodeResult::Type UBTT_BossReset::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();

	if (AIController && AIController->BrainComponent)
	{
		AIController->BrainComponent->StopLogic(TEXT("Stopped by BT Task 'Stop Behavior Tree'"));
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
