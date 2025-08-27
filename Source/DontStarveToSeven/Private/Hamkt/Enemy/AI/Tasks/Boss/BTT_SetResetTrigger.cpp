// Fill out your copyright notice in the Description page of Project Settings.


#include "Hamkt/Enemy/AI/Tasks/Boss/BTT_SetResetTrigger.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Hamkt/Enemy/EnemyBoss/EnemyBossBase.h"

UBTT_SetResetTrigger::UBTT_SetResetTrigger()
{
	NodeName = TEXT("UBTT_SetResetTrigger");
}

EBTNodeResult::Type UBTT_SetResetTrigger::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!IsValid(AIController)) return EBTNodeResult::Failed;

	APawn* AIPawn = AIController->GetPawn();
	if (!IsValid(AIPawn)) return EBTNodeResult::Failed;

	AEnemyBossBase* EnemyBossPawn = Cast<AEnemyBossBase>(AIPawn);
	if (!IsValid(EnemyBossPawn)) return EBTNodeResult::Failed;

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!IsValid(BlackboardComp)) return EBTNodeResult::Failed;

	// 보스 리셋 완료.
	BlackboardComp->SetValueAsBool("bResetTrigger", false);
	
	return EBTNodeResult::Succeeded;
}
