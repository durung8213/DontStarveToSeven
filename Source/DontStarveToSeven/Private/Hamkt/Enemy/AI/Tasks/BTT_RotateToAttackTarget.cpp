// Fill out your copyright notice in the Description page of Project Settings.


#include "Hamkt/Enemy/AI/Tasks/BTT_RotateToAttackTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Pawn.h"

UBTT_RotateToAttackTarget::UBTT_RotateToAttackTarget()
{
	NodeName = TEXT("Rotate To Attack Target");
	bNotifyTick = true;	// TickTask가 호출

	RotationInterpSpeed = 5.f;
	AcceptableAngleThreshold = 5.f;	// 5도 이내면 회전 완료
}

EBTNodeResult::Type UBTT_RotateToAttackTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!IsValid(BlackboardComp)) return EBTNodeResult::Failed;

	AActor* AttackTarget = Cast<AActor>(BlackboardComp->GetValueAsObject(AttackTargetKey.SelectedKeyName));
	if (!IsValid(AttackTarget)) return EBTNodeResult::Failed;

	// 작업은 ticktask에서 처리
	return EBTNodeResult::InProgress;
}

void UBTT_RotateToAttackTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!IsValid(AIController))
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    APawn* AIPawn = AIController->GetPawn();
    if (!IsValid(AIPawn))
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!IsValid(BlackboardComp))
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    AActor* AttackTarget = Cast<AActor>(BlackboardComp->GetValueAsObject(AttackTargetKey.SelectedKeyName));
    if (!IsValid(AttackTarget))
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    // AI Pawn과 타깃 사이의 LookAt 회전 계산
    FVector AILocation = AIPawn->GetActorLocation();
    FVector TargetLocation = AttackTarget->GetActorLocation();
    FRotator DesiredRot = UKismetMathLibrary::FindLookAtRotation(AILocation, TargetLocation);

    // 현재 컨트롤러와 회전과 DesiredRot 사이를 보간
    FRotator CurrentRot = AIController->GetControlRotation();
    FRotator NewRot = FMath::RInterpTo(CurrentRot, DesiredRot, DeltaSeconds, RotationInterpSpeed);

    // 업데이트된 회전값을 적용
    AIController->SetControlRotation(NewRot);

    // 회전완료 조건 : 현재 회전과 DesiredRot 사이 yaw값 차이가 AcceptableAngleThreshold보다 작으면 완료
    float YawDiff = FMath::Abs(NewRot.Yaw - DesiredRot.Yaw);
    if (YawDiff < AcceptableAngleThreshold)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}
