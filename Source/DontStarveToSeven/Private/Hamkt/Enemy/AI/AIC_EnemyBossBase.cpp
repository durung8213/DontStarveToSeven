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
		// 보스 기상
		EnemyBossBase->OnWakeUp.RemoveDynamic(this, &AAIC_EnemyBossBase::HandleWakeUp);
		EnemyBossBase->OnWakeUp.AddDynamic(this, &AAIC_EnemyBossBase::HandleWakeUp);
	}

	AActor* BossZoneBaseIn = UGameplayStatics::GetActorOfClass(this, ABossZoneBase::StaticClass());
	BossZoneBase = Cast<ABossZoneBase>(BossZoneBaseIn);
}

// 매개변수는 가장 가까운 대상 / 가장 멀리있는 대상을 고름
void AAIC_EnemyBossBase::SelectAttackTarget(const FName& SelectTarget)
{
	if (!BossZoneBase)
		return;

	FVector MyLocation = EnemyBossBase->GetActorLocation();

	ACharacter* ClosestPlayer = nullptr;		// AI 와 가장 가까이 있는 대상
	ACharacter* FarthestPlayer = nullptr;	// AI와 가장 멀리 있는 대상
	float MinDistance = FLT_MAX;
	float MaxDistance = -1.f;

	// PlayerArea의 OverlappingPlayers TSet을 사용하여 각 플레이어와의 거리를 계산
	for (ACharacter* Player : BossZoneBase->GetOverlappingPlayers())
	{
		if (Player)
		{
			float Distance = FVector::Dist(MyLocation, Player->GetActorLocation());
			// 가장 가까이 있는 대상
			if (Distance < MinDistance)
			{
				MinDistance = Distance;
				ClosestPlayer = Player;
			}
			// 가장 멀리 있는 대상
			if (Distance > MaxDistance)
			{
				MaxDistance = Distance;
				FarthestPlayer = Player;
			}
		}
	}

	// 가장 가까이있는 대상
	if ((SelectTarget == TEXT("Closest")) && ClosestPlayer)
	{
		UE_LOG(LogTemp, Log, TEXT("Setting Normal Attack Target: %s at distance %f"), *ClosestPlayer->GetName(), MinDistance);
		BlackboardComp->SetValueAsObject(TEXT("AttackTarget"), ClosestPlayer);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No valid target found for Normal Attack."));
	}
	// 가장 멀리있는 대상 키값
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
		BlackboardComp = GetBlackboardComponent();		// 현재 블랙보드 컴포넌트 가져오기
	}

	// 어택 타겟은 보스로부터 가장 가까운 대상
	if (BlackboardComp)
	{
		SelectAttackTarget("Closest");
	}
}
