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
	// ����(ƽ) ������ 1�ʷ� ����
	Interval = 1.f;
	// �ʱ� Blackboard Ű �̸�
	PlayerLocationKey.SelectedKeyName = "PlayerLocation";
}

void UBTS_UpdatePlayerLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!IsValid(BlackboardComp)) return;
   
    // ��� �÷��̾� ĳ����(��: APlayerCharacter Ŭ����)�� �����ɴϴ�.
    TArray<AActor*> PlayerActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABasePlayerCharacter::StaticClass(), PlayerActors);

    AActor* NearestPlayer = nullptr;
    float NearestDistanceSq = FLT_MAX;

    APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
    if (!IsValid(AIPawn)) return;

    FVector AILocation = AIPawn->GetActorLocation();

    // �� �÷��̾���� �Ÿ��� ����Ͽ� ���� ����� �÷��̾� ����
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
        // ���� ����� �÷��̾��� ��ġ�� Blackboard�� ������Ʈ
        BlackboardComp->SetValueAsVector(PlayerLocationKey.SelectedKeyName, NearestPlayer->GetActorLocation());
    }
    
}
