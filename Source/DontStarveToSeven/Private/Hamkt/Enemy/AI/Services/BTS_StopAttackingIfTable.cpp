// Fill out your copyright notice in the Description page of Project Settings.


#include "Hamkt/Enemy/AI/Services/BTS_StopAttackingIfTable.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Actor.h"
#include "Hamkt/Enemy/EnemyBase.h"
#include "Hamkt/Enemy/AI/AIC_EnemyBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/World.h"
#include "GameFramework/PlayerStart.h"

#include "EngineUtils.h" // TActorIterator 사용을 위한 헤더

UBTS_StopAttackingIfTable::UBTS_StopAttackingIfTable()
{
	NodeName = "BTS_StopAttacking If TargetIsDead";
}

void UBTS_StopAttackingIfTable::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// 1. 유효성 검사
	// 
	// BlackboardComponent 검증 및 가져오기
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!IsValid(AIController)) return;

	// OwnerComp에서 블랙보드 컴포넌트를 가져옴
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!IsValid(BlackboardComp)) return;

	// 블랙보드에서 AttackTarget 키값 가져오기
	AActor* AttackTarget = Cast<AActor>(BlackboardComp->GetValueAsObject(AttackTargetKey.SelectedKeyName));
	if (!IsValid(AttackTarget)) return;

	// EnemyBase로 형변환
	AEnemyBase* EnemyTarget = Cast<AEnemyBase>(AttackTarget);
	if (!IsValid(EnemyTarget)) return;

	AAIC_EnemyBase* AIC_EnemyBase = Cast<AAIC_EnemyBase>(AIController);
	if (!IsValid(AIC_EnemyBase)) return;

	// 2. 대상이 사망했는지?

	if (EnemyTarget->IsDead_Implementation())
	{
		AIC_EnemyBase->SetStateAsPassive();
	}
	
}
