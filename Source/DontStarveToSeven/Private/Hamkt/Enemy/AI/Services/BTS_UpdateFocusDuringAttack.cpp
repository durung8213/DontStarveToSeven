// Fill out your copyright notice in the Description page of Project Settings.


#include "Hamkt/Enemy/AI/Services/BTS_UpdateFocusDuringAttack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"
#include "Kismet/KismetMathLibrary.h"


UBTS_UpdateFocusDuringAttack::UBTS_UpdateFocusDuringAttack()
{
	// ���� ������Ʈ �ϵ��� ª�� ����
	Interval = 0.2f;
	
	// TickNode�� ȣ��
	bNotifyTick = true;
	NodeName = TEXT("Update Focus During Attack");
}

void UBTS_UpdateFocusDuringAttack::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!IsValid(AIController)) return;

	APawn* AIPawn = AIController->GetPawn();
	if (!IsValid(AIPawn)) return;

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!IsValid(BlackboardComp)) return;

	AActor* AttackTarget = Cast<AActor>(BlackboardComp->GetValueAsObject(AttackTargetKey.SelectedKeyName));
	if (!IsValid(AttackTarget)) return;

	// ���� pawn ��ġ�� Ÿ�� ��ġ�κ��� lookat ȸ�� ���
	FVector AILocation = AIPawn->GetActorLocation();
	FVector TargetLocation = AttackTarget->GetActorLocation();
	FRotator DisiredRot = UKismetMathLibrary::FindLookAtRotation(AILocation, TargetLocation);

	// ���� ��Ʈ�ѷ� ȸ���� DesiredRot�� ����(interp)�Ͽ� �ε巴�� ��ȯ
	FRotator CurrentRot = AIController->GetControlRotation();
	FRotator NewRot = FMath::RInterpTo(CurrentRot, DisiredRot, DeltaSeconds, RotationInterpSpeed);

	// ������Ʈ�� ȸ������ ��Ʈ�ѷ��� ���� (ĳ���Ͱ� �̸� ���� ȸ���ؾ���)
	AIController->SetControlRotation(NewRot);


}
