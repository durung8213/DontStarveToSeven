 // Fill out your copyright notice in the Description page of Project Settings.


#include "Hamkt/Enemy/AI/AIC_EnemyBossBase.h"
#include "Hamkt/Enemy/EnemyBoss/EnemyBossBase.h"
#include "Hamkt/Actor/BossResetLocation.h"
#include "Hamkt/Actor/BossZoneBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

AAIC_EnemyBossBase::AAIC_EnemyBossBase()
{
}

void AAIC_EnemyBossBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	EnemyBossBase = Cast<AEnemyBossBase>(InPawn);
	if (EnemyBossBase)
	{
		// ���� ���
		EnemyBossBase->OnWakeUp.RemoveDynamic(this, &AAIC_EnemyBossBase::HandleWakeUp);
		EnemyBossBase->OnWakeUp.AddDynamic(this, &AAIC_EnemyBossBase::HandleWakeUp);
	}

	AActor* BossZoneBaseIn = UGameplayStatics::GetActorOfClass(this, ABossZoneBase::StaticClass());
	BossZoneBase = Cast<ABossZoneBase>(BossZoneBaseIn);
}

// �Ű������� ���� ����� ��� / ���� �ָ��ִ� ����� ��
void AAIC_EnemyBossBase::SelectAttackTarget(const FName& SelectTarget)
{
	if (!BossZoneBase)
		return;

	FVector MyLocation = EnemyBossBase->GetActorLocation();

	ACharacter* ClosestPlayer = nullptr;		// AI �� ���� ������ �ִ� ���
	ACharacter* FarthestPlayer = nullptr;	// AI�� ���� �ָ� �ִ� ���
	float MinDistance = FLT_MAX;
	float MaxDistance = -1.f;

	// PlayerArea�� OverlappingPlayers TSet�� ����Ͽ� �� �÷��̾���� �Ÿ��� ���
	for (ACharacter* Player : BossZoneBase->GetOverlappingPlayers())
	{
		if (Player)
		{
			float Distance = FVector::Dist(MyLocation, Player->GetActorLocation());
			// ���� ������ �ִ� ���
			if (Distance < MinDistance)
			{
				MinDistance = Distance;
				ClosestPlayer = Player;
			}
			// ���� �ָ� �ִ� ���
			if (Distance > MaxDistance)
			{
				MaxDistance = Distance;
				FarthestPlayer = Player;
			}
		}
	}

	// ���� �������ִ� ���
	if ((SelectTarget == TEXT("Closest")) && ClosestPlayer)
	{
		UE_LOG(LogTemp, Log, TEXT("Setting Normal Attack Target: %s at distance %f"), *ClosestPlayer->GetName(), MinDistance);
		BlackboardComp->SetValueAsObject(TEXT("AttackTarget"), ClosestPlayer);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No valid target found for Normal Attack."));
	}
	// ���� �ָ��ִ� ��� Ű��
	if ((SelectTarget == TEXT("Farthest")) && FarthestPlayer)
	{
		UE_LOG(LogTemp, Log, TEXT("Setting Special Attack Target: %s at distance %f"), *FarthestPlayer->GetName(), MaxDistance);
		BlackboardComp->SetValueAsObject(TEXT("AttackTarget"), FarthestPlayer);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No valid target found for Special Attack."));
	}
}

void AAIC_EnemyBossBase::HandleWakeUp()
{
	if (RunBehaviorTree(EnemyBossBase->GetBehaviorTree()))
	{
		BlackboardComp = GetBlackboardComponent();		// ���� ������ ������Ʈ ��������
	}

	// ���� Ÿ���� �����κ��� ���� ����� ���
	if (BlackboardComp)
	{
		SelectAttackTarget("Closest");
	}
}
