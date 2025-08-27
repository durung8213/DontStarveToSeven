// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_UpdateTarget.generated.h"

/**
 * 플레이어를 추적중 앞에 라인트레이스에 craft가 있다면 공격대상을 craft로 전환
 */
UCLASS()
class DONTSTARVETOSEVEN_API UBTS_UpdateTarget : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTS_UpdateTarget();
	
protected:
	// 매개변수 - BehaviorTreeComponent 참조 / 노드 메모리 / 틱 간격
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	// Blackboard의 타깃 오브젝트를 저장
	UPROPERTY(EditAnywhere, Category = "Keys")
	FBlackboardKeySelector AttackTargetKey;

	// Craft 대상 감지를 위한 반경
	UPROPERTY(EditAnywhere, Category = "Detection")
	float CraftDetectionRadius;


};
