// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_RotateToAttackTarget.generated.h"

/**
 *AI가 Blackboard의 AttackTarget을 향해 회전하도록 하는 BT Task.
 *        매 Tick마다 현재 회전과 타깃 방향 사이의 차이를 보간(interp)하며, 
 *        임계각도 이하가 되면 작업을 완료합니다. 
 */
UCLASS()
class DONTSTARVETOSEVEN_API UBTT_RotateToAttackTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_RotateToAttackTarget();

	// task가 시작될 때 호출
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// 매 틱마다 호출
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	// 블랙보드의 attacktarget 키 참조
	UPROPERTY(EditAnywhere, Category = "Keys")
	FBlackboardKeySelector AttackTargetKey;

	// 회전 보간 속도 ( 높을 수록 회전이 빠르게 업데이트 )
	UPROPERTY(EditAnywhere, Category = "Rotation")
	float RotationInterpSpeed;

	// 회전이 완료를 알려주는 임계각도
	UPROPERTY(EditAnywhere, Category = "Rotation")
	float AcceptableAngleThreshold;

private:
	
};
