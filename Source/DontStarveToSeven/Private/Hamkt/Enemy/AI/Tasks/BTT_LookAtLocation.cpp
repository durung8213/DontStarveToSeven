// Fill out your copyright notice in the Description page of Project Settings.


#include "Hamkt/Enemy/AI/Tasks/BTT_LookAtLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"
#include "Kismet/KismetMathLibrary.h"

UBTT_LookAtLocation::UBTT_LookAtLocation()
{
    NodeName = TEXT("Look At Location");
    LookAtLocationKey.SelectedKeyName = TEXT("LookAtLocation");
}

EBTNodeResult::Type UBTT_LookAtLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AICon = OwnerComp.GetAIOwner();
    if (!AICon) return EBTNodeResult::Failed;

    APawn* AIPawn = AICon->GetPawn();
    if (!AIPawn) return EBTNodeResult::Failed;

    UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
    if (!BBComp) return EBTNodeResult::Failed;

    // 1) Blackboard���� Vector ��ġ ������
    FVector TargetLocation = BBComp->GetValueAsVector(LookAtLocationKey.SelectedKeyName);

    if (TargetLocation == FVector::ZeroVector)
    {
        // ��ġ�� ��ȿ���� �ʴٸ�
        return EBTNodeResult::Failed;
    }

    // 2) FindLookAtRotation
    FVector AILocation = AIPawn->GetActorLocation();
    FRotator DesiredRot = UKismetMathLibrary::FindLookAtRotation(AILocation, TargetLocation);

    // 3) ��Ʈ�ѷ� ȸ�� ����
    AICon->SetControlRotation(DesiredRot);

    return EBTNodeResult::Succeeded;
}