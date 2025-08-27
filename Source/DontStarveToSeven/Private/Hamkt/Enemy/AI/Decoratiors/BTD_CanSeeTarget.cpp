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
	// 유효성 검사
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!IsValid(AIController)) return false;

	// OwnerComp에서 블랙보드 컴포넌트를 가져옴
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!IsValid(BlackboardComp)) return false;

	// 블랙보드에서 AttackTarget 키값 가져오기
	AActor* AttackTarget = Cast<AActor>(BlackboardComp->GetValueAsObject(AttackTargetKey.SelectedKeyName));
	if (!IsValid(AttackTarget)) return false;

	// AI의 Pawn 가져오기
	APawn* AIPawn = AIController->GetPawn();
	if (!IsValid(AIPawn)) return false;


	///** LineTraceByChannel **///

	// 1. 시작 위치(Start)와 끝 위치(End)
	FVector StartLocation = AIPawn->GetActorLocation();
	FVector EndLocation = AttackTarget->GetActorLocation();

	// 2. 라인트레이스 결과를 저장할 FHitResult
	FHitResult HitResult;

	// 3. 충돌 쿼리 파라미터 설정 - 자기자신 무시
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(AIPawn);

	// 4. 라인트레이스 함수 호출
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,		// 결과 저장 변수
		StartLocation,	// 시작 위치
		EndLocation,	// 끝 위치
		ECC_Visibility,	// 충돌 채널
		QueryParams		// 쿼리 파라미터
	);

	// 5. 디버그 라인 (충돌하면 빨강, 그 외 초록)
	//DrawDebugLine(
	//	AIPawn->GetWorld(),
	//	StartLocation,
	//	EndLocation,
	//	bHit ? FColor::Red : FColor::Green,
	//	false, // 영구 표시 여부
	//	1.0f,  // 지속 시간 (1초)
	//	0,     // 우선순위
	//	2.0f   // 선 두께
	//);

	// 6. 충돌하지 않았다면 true
	bool bReturnValue = !bHit;

	return bReturnValue;
}
