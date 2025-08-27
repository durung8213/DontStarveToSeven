// Fill out your copyright notice in the Description page of Project Settings.


#include "Hamkt/Enemy/AI/Services/BTS_UpdatePlayerLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Hamkt/Player/HKT_Player.h"
#include "AIController.h"
#include "Kimmh/BasePlayerCharacter.h"

UBTS_UpdatePlayerLocation::UBTS_UpdatePlayerLocation()
{
	// 서비스(틱) 간격을 1초로 설정
	Interval = 1.f;
	// 초기 Blackboard 키 이름
	PlayerLocationKey.SelectedKeyName = "PlayerLocation";
}

void UBTS_UpdatePlayerLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!IsValid(BlackboardComp)) return;
   
    // 모든 플레이어 캐릭터(예: APlayerCharacter 클래스)를 가져옵니다.
    TArray<AActor*> PlayerActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABasePlayerCharacter::StaticClass(), PlayerActors);

    AActor* NearestPlayer = nullptr;
    float NearestDistanceSq = FLT_MAX;

    APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
    if (!IsValid(AIPawn)) return;

    FVector AILocation = AIPawn->GetActorLocation();

    // 각 플레이어와의 거리를 계산하여 가장 가까운 플레이어 선택
    for (AActor* Player : PlayerActors)
    {
        if (!Player)
        {
            continue;
        }

        float DistanceSq = FVector::DistSquared(Player->GetActorLocation(), AILocation);
        if (DistanceSq < NearestDistanceSq)
        {
            NearestDistanceSq = DistanceSq;
            NearestPlayer = Player;
        }
    }

    if (NearestPlayer)
    {
        // 가장 가까운 플레이어의 위치를 Blackboard에 업데이트
        BlackboardComp->SetValueAsVector(PlayerLocationKey.SelectedKeyName, NearestPlayer->GetActorLocation());
    }
    
}
