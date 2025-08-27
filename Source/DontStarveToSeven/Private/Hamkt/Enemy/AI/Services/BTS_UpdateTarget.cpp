#include "Hamkt/Enemy/AI/Services/BTS_UpdateTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "DrawDebugHelpers.h"  // ����� ���� Ȯ�ο�
#include "Hamkt/Player/HKT_Player.h"
#include "Kimmh/BasePlayerCharacter.h"

UBTS_UpdateTarget::UBTS_UpdateTarget()
{
    // ���� ���� ����
    Interval = 1.f;

    // ������ Ű �̸� ���� (Ű Ÿ���� �ݵ�� Object�� �����Ǿ� �־�� ��)
    AttackTargetKey.SelectedKeyName = "AttackTarget";

    // ���� �ݰ� (600cm)
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

    // ���� AI�� �����̼�
    FVector AILocation = AIPawn->GetActorLocation();

    // 1. ���� �ݰ� ���� �÷��̾���� ����
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

    // �ۼ��ǿ��� �÷��̾ �����Ǿ�����!

    // 2. �÷��̾ �����Ǿ�����, AI�� �÷��̾� ���̿� ���� Ʈ���̽� ����
    if (NearestPlayer)
    {
        FHitResult HitResult;
        FVector PlayerLocation = NearestPlayer->GetActorLocation();
        FCollisionQueryParams QueryParams;
        QueryParams.AddIgnoredActor(AIPawn);  // �ڱ� �ڽ� ����

        bool bHit = GetWorld()->LineTraceSingleByChannel(
            HitResult,
            AILocation,         // Start
            PlayerLocation,     // End
            ECC_Visibility,     // ���� ä��
            QueryParams
        );

        // ����� ���� (�׽�Ʈ �� ���� ����)
        //DrawDebugLine(GetWorld(), AILocation, PlayerLocation, FColor::Green, false, 0.5f, 0, 2.f);

        // ���� ��ο� Craft�� ���� ������, Ÿ���� Craft�� ����
        if (bHit && HitResult.GetActor() && HitResult.GetActor()->ActorHasTag(FName("Craft")))
        {
            BlackboardComp->SetValueAsObject(AttackTargetKey.SelectedKeyName, HitResult.GetActor());
        }
        else
        {
            // ��ο� �ƹ��͵� ���ٸ�, �÷��̾ Ÿ������ ����
            BlackboardComp->SetValueAsObject(AttackTargetKey.SelectedKeyName, NearestPlayer);
        }
    }
    else
    {
        // 3. �÷��̾ �������� �ʾ�����, Craft�� ����
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
            // ����� �ϳ��� ���ٸ� Ÿ���� Ŭ����
            BlackboardComp->ClearValue(AttackTargetKey.SelectedKeyName);
        }
    }

    // �� �߰�: AttackTarget�� �����Ǿ����� AIController�� ��Ŀ���� �ش� Ÿ������ ������Ʈ
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