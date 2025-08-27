// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_RotateToAttackTarget.generated.h"

/**
 *AI�� Blackboard�� AttackTarget�� ���� ȸ���ϵ��� �ϴ� BT Task.
 *        �� Tick���� ���� ȸ���� Ÿ�� ���� ������ ���̸� ����(interp)�ϸ�, 
 *        �Ӱ谢�� ���ϰ� �Ǹ� �۾��� �Ϸ��մϴ�. 
 */
UCLASS()
class DONTSTARVETOSEVEN_API UBTT_RotateToAttackTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_RotateToAttackTarget();

	// task�� ���۵� �� ȣ��
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// �� ƽ���� ȣ��
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	// �������� attacktarget Ű ����
	UPROPERTY(EditAnywhere, Category = "Keys")
	FBlackboardKeySelector AttackTargetKey;

	// ȸ�� ���� �ӵ� ( ���� ���� ȸ���� ������ ������Ʈ )
	UPROPERTY(EditAnywhere, Category = "Rotation")
	float RotationInterpSpeed;

	// ȸ���� �ϷḦ �˷��ִ� �Ӱ谢��
	UPROPERTY(EditAnywhere, Category = "Rotation")
	float AcceptableAngleThreshold;

private:
	
};
