


/**  현재 AI MOVE TO 코드로 구현 할 때의 문제가 많이 있어서 해당 Task는 내용은 블루프린트로 구현 **/


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_MoveToIdealRange.generated.h"

/**
 * 
 */
UCLASS()
class DONTSTARVETOSEVEN_API UBTT_MoveToIdealRange : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_MoveToIdealRange();

	// Task가 실행 될 떄 호출
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


private:
	// 블랙보드에서 공격 대상(Actor)를 가져올 키
	UPROPERTY(EditAnywhere, Category = "Keys", meta = (AllowPrivateAccess = "true"))
	FBlackboardKeySelector IdealRangeKey;

	// 블랙보드에서 공격 사거리를 가져올 키
	UPROPERTY(EditAnywhere, Category = "Keys", meta = (AllowPrivateAccess = "true"))
	FBlackboardKeySelector AttackTargetKey;
	
};
