// Fill out your copyright notice in the Description page of Project Settings.


#include "Hamkt/Enemy/AI/Tasks/Boss/BTT_BossMontagePlay.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Hamkt/Enemy/EnemyBoss/EnemyBossBase.h"
#include "AIController.h"
#include "Hamkt/Enemy/AI/AIC_EnemyBossBase.h"
#include "GameFramework/Pawn.h"
#include "Engine/Engine.h"

UBTT_BossMontagePlay::UBTT_BossMontagePlay()
{
	NodeName = TEXT("BTT_BossMontagePlay");
	CachedOwnerComp = nullptr;
}

EBTNodeResult::Type UBTT_BossMontagePlay::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!IsValid(AIController)) return EBTNodeResult::Failed;

	AAIC_EnemyBossBase* AICEnemyBossBase = Cast<AAIC_EnemyBossBase>(AIController);

	APawn* AIPawn = AIController->GetPawn();
	if (!IsValid(AIPawn)) return EBTNodeResult::Failed;

	AEnemyBossBase* EnemyBossPawn = Cast<AEnemyBossBase>(AIPawn);
	if (!IsValid(EnemyBossPawn)) return EBTNodeResult::Failed;

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!IsValid(BlackboardComp)) return EBTNodeResult::Failed;

	// BT Task 완료를 위해 오너컴퍼넌트 캐싱
	CachedOwnerComp = &OwnerComp;

	EnemyBossPawn->ServerRoarMontage();

	EnemyBossPawn->OnBossAttackEnd.RemoveDynamic(this, &UBTT_BossMontagePlay::OnMontageFinished);
	EnemyBossPawn->OnBossAttackEnd.AddDynamic(this, &UBTT_BossMontagePlay::OnMontageFinished);

	return EBTNodeResult::InProgress;

}

void UBTT_BossMontagePlay::OnMontageFinished(UAnimMontage* Montage, bool bInterrupted)
{
	if (CachedOwnerComp)
	{
		AAIController* AIController = CachedOwnerComp->GetAIOwner();
		if (AIController)
		{
			AEnemyBossBase* EnemyBossPawn = Cast<AEnemyBossBase>(AIController->GetPawn());
			if (EnemyBossPawn)
			{
				// 여러번 호출 방지 위해서 바인딩 해제
				EnemyBossPawn->OnBossAttackEnd.RemoveDynamic(this, &UBTT_BossMontagePlay::OnMontageFinished);
			}
		}
		// task 완료
		FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
	}
}
