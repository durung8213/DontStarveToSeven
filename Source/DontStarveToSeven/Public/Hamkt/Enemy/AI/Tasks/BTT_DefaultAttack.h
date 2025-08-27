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

	// Task�� ����� �� ȣ��˴ϴ�.
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// ���� ���� ��(OnAttackEnd �̺�Ʈ�� ���� ȣ���) ������ Ŀ���� �̺�Ʈ
	UFUNCTION()
	void FinishedAttacking();
	
private:
	UPROPERTY()
	UBehaviorTreeComponent* CachedOwnerComp;
};
