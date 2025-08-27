// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_DefaultAttack.generated.h"

/**
 * 
 */
UCLASS()
class DONTSTARVETOSEVEN_API UBTT_DefaultAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_DefaultAttack();

	// Task가 실행될 때 호출됩니다.
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// 공격 종료 시(OnAttackEnd 이벤트에 의해 호출됨) 실행할 커스텀 이벤트
	UFUNCTION()
	void FinishedAttacking();
	
private:
	UPROPERTY()
	UBehaviorTreeComponent* CachedOwnerComp;
};
