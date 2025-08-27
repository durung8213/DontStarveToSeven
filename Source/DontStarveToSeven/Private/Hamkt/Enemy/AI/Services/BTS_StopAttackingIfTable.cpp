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

#include "EngineUtils.h" // TActorIterator ����� ���� ���

UBTS_StopAttackingIfTable::UBTS_StopAttackingIfTable()
{
	NodeName = "BTS_StopAttacking If TargetIsDead";
}

void UBTS_StopAttackingIfTable::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// 1. ��ȿ�� �˻�
	// 
	// BlackboardComponent ���� �� ��������
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!IsValid(AIController)) return;

	// OwnerComp���� ������ ������Ʈ�� ������
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!IsValid(BlackboardComp)) return;

	// �����忡�� AttackTarget Ű�� ��������
	AActor* AttackTarget = Cast<AActor>(BlackboardComp->GetValueAsObject(AttackTargetKey.SelectedKeyName));
	if (!IsValid(AttackTarget)) return;

	// EnemyBase�� ����ȯ
	AEnemyBase* EnemyTarget = Cast<AEnemyBase>(AttackTarget);
	if (!IsValid(EnemyTarget)) return;

	AAIC_EnemyBase* AIC_EnemyBase = Cast<AAIC_EnemyBase>(AIController);
	if (!IsValid(AIC_EnemyBase)) return;

	// 2. ����� ����ߴ���?

	if (EnemyTarget->IsDead_Implementation())
	{
		AIC_EnemyBase->SetStateAsPassive();
	}
	
}
