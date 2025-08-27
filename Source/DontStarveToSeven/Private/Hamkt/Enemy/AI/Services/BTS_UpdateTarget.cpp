#include "Hamkt/Enemy/AI/Services/BTS_UpdateTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "DrawDebugHelpers.h"  // 디버그 라인 확인용
#include "Hamkt/Player/HKT_Player.h"
#include "Kimmh/BasePlayerCharacter.h"

UBTS_UpdateTarget::UBTS_UpdateTarget()
{
    // 서비스 실행 간격
    Interval = 1.f;

    // 블랙보드 키 이름 저장 (키 타입은 반드시 Object로 설정되어 있어야 함)
    AttackTargetKey.SelectedKeyName = "AttackTarget";

    // 감지 반경 (600cm)
    CraftDetectionRadius = 600.f;
}

void UBTS_UpdateTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!IsValid(BlackboardComp)) return;

    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!IsValid(AIController)) return;

    APawn* AIPawn = AIController->GetPawn();
    if (!IsValid(AIPawn)) return;

    // 현재 AI의 로케이션
    FVector AILocation = AIPawn->GetActorLocation();

    // 1. 감지 반경 내의 플레이어들을 감지
    TArray<AActor*> PlayerActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseCharacter::StaticClass(), PlayerActors);

    AActor* NearestPlayer = nullptr;
    float NearestPlayerDistSq = FLT_MAX;
    for (AActor* Player : PlayerActors)
    {
        if (!Player) continue;

        float DistSq = FVector::DistSquared(Player->GetActorLocation(), AILocation);
        if (DistSq <= CraftDetectionRadius * CraftDetectionRadius && DistSq < NearestPlayerDistSq)
        {
            NearestPlayer = Player;
            NearestPlayerDistSq = DistSq;
        }
    }

    // 퍼셉션에서 플레이어가 감지되었으면!

    // 2. 플레이어가 감지되었으면, AI와 플레이어 사이에 라인 트레이스 수행
    if (NearestPlayer)
    {
        FHitResult HitResult;
        FVector PlayerLocation = NearestPlayer->GetActorLocation();
        FCollisionQueryParams QueryParams;
        QueryParams.AddIgnoredActor(AIPawn);  // 자기 자신 무시

        bool bHit = GetWorld()->LineTraceSingleByChannel(
            HitResult,
            AILocation,         // Start
            PlayerLocation,     // End
            ECC_Visibility,     // 감지 채널
            QueryParams
        );

        // 디버그 라인 (테스트 후 제거 가능)
        //DrawDebugLine(GetWorld(), AILocation, PlayerLocation, FColor::Green, false, 0.5f, 0, 2.f);

        // 만약 경로에 Craft가 막혀 있으면, 타깃을 Craft로 변경
        if (bHit && HitResult.GetActor() && HitResult.GetActor()->ActorHasTag(FName("Craft")))
        {
            BlackboardComp->SetValueAsObject(AttackTargetKey.SelectedKeyName, HitResult.GetActor());
        }
        else
        {
            // 경로에 아무것도 없다면, 플레이어를 타깃으로 설정
            BlackboardComp->SetValueAsObject(AttackTargetKey.SelectedKeyName, NearestPlayer);
        }
    }
    else
    {
        // 3. 플레이어가 감지되지 않았으면, Craft를 감지
        TArray<AActor*> CraftActors;
        UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Craft"), CraftActors);

        AActor* NearestCraft = nullptr;
        float NearestCraftDistSq = FLT_MAX;

        for (AActor* Actor : CraftActors)
        {
            if (!Actor) continue;

            float DistSq = FVector::DistSquared(Actor->GetActorLocation(), AILocation);
            if (DistSq <= CraftDetectionRadius * CraftDetectionRadius && DistSq < NearestCraftDistSq)
            {
                NearestCraft = Actor;
                NearestCraftDistSq = DistSq;
            }
        }

        if (NearestCraft)
        {
            BlackboardComp->SetValueAsObject(AttackTargetKey.SelectedKeyName, NearestCraft);
            return;
        }
        else
        {
            // 대상이 하나도 없다면 타깃을 클리어
            BlackboardComp->ClearValue(AttackTargetKey.SelectedKeyName);
        }
    }

    // ★ 추가: AttackTarget이 설정되었으면 AIController의 포커스를 해당 타깃으로 업데이트
    AActor* NewTarget = Cast<AActor>(BlackboardComp->GetValueAsObject(AttackTargetKey.SelectedKeyName));
    if (IsValid(NewTarget))
    {
        AIController->SetFocus(NewTarget);
    }
    else
    {
        AIController->ClearFocus(EAIFocusPriority::Gameplay);
    }
}