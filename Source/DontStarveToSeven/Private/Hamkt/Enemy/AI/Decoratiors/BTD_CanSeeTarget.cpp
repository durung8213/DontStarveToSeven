// Fill out your copyright notice in the Description page of Project Settings.


#include "Hamkt/Enemy/AI/Decoratiors/BTD_CanSeeTarget.h"
#include "AIController.h"
#include "Hamkt/Enemy/AI/AIC_EnemyBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Actor.h"

UBTD_CanSeeTarget::UBTD_CanSeeTarget()
{
	NodeName = TEXT("BTD_CanSeeTarget");
}

bool UBTD_CanSeeTarget::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	// ��ȿ�� �˻�
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!IsValid(AIController)) return false;

	// OwnerComp���� ������ ������Ʈ�� ������
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!IsValid(BlackboardComp)) return false;

	// �����忡�� AttackTarget Ű�� ��������
	AActor* AttackTarget = Cast<AActor>(BlackboardComp->GetValueAsObject(AttackTargetKey.SelectedKeyName));
	if (!IsValid(AttackTarget)) return false;

	// AI�� Pawn ��������
	APawn* AIPawn = AIController->GetPawn();
	if (!IsValid(AIPawn)) return false;


	///** LineTraceByChannel **///

	// 1. ���� ��ġ(Start)�� �� ��ġ(End)
	FVector StartLocation = AIPawn->GetActorLocation();
	FVector EndLocation = AttackTarget->GetActorLocation();

	// 2. ����Ʈ���̽� ����� ������ FHitResult
	FHitResult HitResult;

	// 3. �浹 ���� �Ķ���� ���� - �ڱ��ڽ� ����
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(AIPawn);

	// 4. ����Ʈ���̽� �Լ� ȣ��
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,		// ��� ���� ����
		StartLocation,	// ���� ��ġ
		EndLocation,	// �� ��ġ
		ECC_Visibility,	// �浹 ä��
		QueryParams		// ���� �Ķ����
	);

	// 5. ����� ���� (�浹�ϸ� ����, �� �� �ʷ�)
	//DrawDebugLine(
	//	AIPawn->GetWorld(),
	//	StartLocation,
	//	EndLocation,
	//	bHit ? FColor::Red : FColor::Green,
	//	false, // ���� ǥ�� ����
	//	1.0f,  // ���� �ð� (1��)
	//	0,     // �켱����
	//	2.0f   // �� �β�
	//);

	// 6. �浹���� �ʾҴٸ� true
	bool bReturnValue = !bHit;

	return bReturnValue;
}
