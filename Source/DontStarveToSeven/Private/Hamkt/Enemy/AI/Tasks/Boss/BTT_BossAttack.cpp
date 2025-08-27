// Fill out your copyright notice in the Description page of Project Settings.


#include "Hamkt/Enemy/AI/Tasks/Boss/BTT_BossAttack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Hamkt/Enemy/EnemyBoss/EnemyBossBase.h"
#include "AIController.h"
#include "Hamkt/Enemy/AI/AIC_EnemyBossBase.h"
#include "GameFramework/Pawn.h"
#include "Engine/Engine.h"

UBTT_BossAttack::UBTT_BossAttack()
{
	NodeName = TEXT("BTT_BossAttack");
	CachedOwnerComp = nullptr;
	bCreateNodeInstance = true;		// ����?
}

EBTNodeResult::Type UBTT_BossAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	// BT Task �ϷḦ ���� �������۳�Ʈ ĳ��
	CachedOwnerComp = &OwnerComp;

	FName AttackKey;
    switch (AttackType)
    {
    case EEnemyAttackType::Attack1:
        AttackKey = FName("Attack1");
        AICEnemyBossBase->SelectAttackTarget("Closest");
        break;
    case EEnemyAttackType::Attack2:
        AttackKey = FName("Attack2");
        break;
    case EEnemyAttackType::Attack3:
        AttackKey = FName("Attack3");
        break;
    case EEnemyAttackType::Attack4:
        AttackKey = FName("Attack4");
        break;
    case EEnemyAttackType::FWAttack1:
        AttackKey = FName("FWAttack1");
        break;
    case EEnemyAttackType::FWAttack2:
        AttackKey = FName("FWAttack2");
        break;
    case EEnemyAttackType::JumpAttack:
    {
        AICEnemyBossBase->SelectAttackTarget("Farthest");
        DashAttack(BlackboardComp, EnemyBossPawn, 4000.f, 400.f);
        AttackKey = FName("JumpAttack");
        
        break;
    }
    case EEnemyAttackType::BiteForward:
        DashAttack(BlackboardComp, EnemyBossPawn, 3000.f, 0.f);
        AttackKey = FName("BiteForward");
        break;
    default:
        AttackKey = FName("Attack1");
        break;
	}

    // EnemyBoss���� Attack �Լ� ȣ�� ( �Լ� ���ο��� �ش� Ű����� �´� ��Ÿ�ָ� ȣ����
    EnemyBossPawn->Attack(AttackKey);
    // ����� �޽��� ���
    if (GEngine)
    {
        FString Message = FString::Printf(TEXT("Attack triggered: %s"), *AttackKey.ToString());
        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, Message);
    }

    EnemyBossPawn->OnBossAttackEnd.RemoveDynamic(this, &UBTT_BossAttack::OnAttackMontageFinished);
    EnemyBossPawn->OnBossAttackEnd.AddDynamic(this, &UBTT_BossAttack::OnAttackMontageFinished);


	return EBTNodeResult::InProgress;
}


void UBTT_BossAttack::OnAttackMontageFinished(UAnimMontage* Montage, bool bInterrupted)
{
    if (CachedOwnerComp)
    {
        AAIController* AIController = CachedOwnerComp->GetAIOwner();
        if (AIController)
        {
            AEnemyBossBase* EnemyBossPawn = Cast<AEnemyBossBase>(AIController->GetPawn());
            if (EnemyBossPawn)
            {
                // ������ ȣ�� ���� ���ؼ� ���ε� ����
                EnemyBossPawn->OnBossAttackEnd.RemoveDynamic(this, &UBTT_BossAttack::OnAttackMontageFinished);
            }
        }
        // task �Ϸ�
        FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
    }
}

void UBTT_BossAttack::DashAttack(UBlackboardComponent* InBlackboardComp, AEnemyBossBase* InEnemyBossPawn, float HorizontalSpeed, float VertialSpeed)
{
    UObject* AttackTargetObj = InBlackboardComp->GetValueAsObject("AttackTarget");
    if (!IsValid(AttackTargetObj)) return;

    AActor* TargetActor = Cast<AActor>(AttackTargetObj);
    if (!IsValid(TargetActor)) return;

    FVector TargetLocation = TargetActor->GetActorLocation();

    if (!TargetLocation.IsNearlyZero())
    {
        // ���� Pawn ��ġ���� ��ǥ������ ���� ��� (����ȭ)
        FVector Direction = (TargetLocation - InEnemyBossPawn->GetActorLocation()).GetSafeNormal();

        // ���� �� 
        // ������ ������ ���� �� ���� �ӵ� (��Ȳ�� ���� ����)
        FVector LaunchVelocity = Direction * HorizontalSpeed;
        LaunchVelocity.Z = VertialSpeed; // 4000 400

        // ���� ���� �̵� ����
        InEnemyBossPawn->LaunchCharacter(LaunchVelocity, true, true);
    }
}