


/**  ���� AI MOVE TO �ڵ�� ���� �� ���� ������ ���� �־ �ش� Task�� ������ �������Ʈ�� ���� **/


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

	// Task�� ���� �� �� ȣ��
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


private:
	// �����忡�� ���� ���(Actor)�� ������ Ű
	UPROPERTY(EditAnywhere, Category = "Keys", meta = (AllowPrivateAccess = "true"))
	FBlackboardKeySelector IdealRangeKey;

	// �����忡�� ���� ��Ÿ��� ������ Ű
	UPROPERTY(EditAnywhere, Category = "Keys", meta = (AllowPrivateAccess = "true"))
	FBlackboardKeySelector AttackTargetKey;
	
};
