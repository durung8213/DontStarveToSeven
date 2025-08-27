// Fill out your copyright notice in the Description page of Project Settings.


#include "Hamkt/Enemy/AI/Tasks/BTT_RotateToAttackTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Pawn.h"

UBTT_RotateToAttackTarget::UBTT_RotateToAttackTarget()
{
	NodeName = TEXT("Rotate To Attack Target");
	bNotifyTick = true;	// TickTask�� ȣ��

	RotationInterpSpeed = 5.f;
	AcceptableAngleThreshold = 5.f;	// 5�� �̳��� ȸ�� �Ϸ�
}

EBTNodeResult::Type UBTT_RotateToAttackTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!IsValid(BlackboardComp)) return EBTNodeResult::Failed;

	AActor* AttackTarget = Cast<AActor>(BlackboardComp->GetValueAsObject(AttackTargetKey.SelectedKeyName));
	if (!IsValid(AttackTarget)) return EBTNodeResult::Failed;

	// �۾��� ticktask���� ó��
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

    // AI Pawn�� Ÿ�� ������ LookAt ȸ�� ���
    FVector AILocation = AIPawn->GetActorLocation();
    FVector TargetLocation = AttackTarget->GetActorLocation();
    FRotator DesiredRot = UKismetMathLibrary::FindLookAtRotation(AILocation, TargetLocation);

    // ���� ��Ʈ�ѷ��� ȸ���� DesiredRot ���̸� ����
    FRotator CurrentRot = AIController->GetControlRotation();
    FRotator NewRot = FMath::RInterpTo(CurrentRot, DesiredRot, DeltaSeconds, RotationInterpSpeed);

    // ������Ʈ�� ȸ������ ����
    AIController->SetControlRotation(NewRot);

    // ȸ���Ϸ� ���� : ���� ȸ���� DesiredRot ���� yaw�� ���̰� AcceptableAngleThreshold���� ������ �Ϸ�
    float YawDiff = FMath::Abs(NewRot.Yaw - DesiredRot.Yaw);
    if (YawDiff < AcceptableAngleThreshold)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}
