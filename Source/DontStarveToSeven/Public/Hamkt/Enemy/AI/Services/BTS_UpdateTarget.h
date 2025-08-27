// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_UpdateTarget.generated.h"

/**
 * �÷��̾ ������ �տ� ����Ʈ���̽��� craft�� �ִٸ� ���ݴ���� craft�� ��ȯ
 */
UCLASS()
class DONTSTARVETOSEVEN_API UBTS_UpdateTarget : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTS_UpdateTarget();
	
protected:
	// �Ű����� - BehaviorTreeComponent ���� / ��� �޸� / ƽ ����
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	// Blackboard�� Ÿ�� ������Ʈ�� ����
	UPROPERTY(EditAnywhere, Category = "Keys")
	FBlackboardKeySelector AttackTargetKey;

	// Craft ��� ������ ���� �ݰ�
	UPROPERTY(EditAnywhere, Category = "Detection")
	float CraftDetectionRadius;


};
