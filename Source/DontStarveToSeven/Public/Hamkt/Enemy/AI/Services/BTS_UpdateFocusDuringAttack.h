// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_UpdateFocusDuringAttack.generated.h"

/**
 * 
 */
UCLASS()
class DONTSTARVETOSEVEN_API UBTS_UpdateFocusDuringAttack : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTS_UpdateFocusDuringAttack();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	// �����忡 ����� ����Ÿ�� Ű
	UPROPERTY(EditAnywhere, Category = "Keys")
	FBlackboardKeySelector AttackTargetKey;

	// ȸ�� �����ӵ� ( ���� ���� ������ ȸ����)
	UPROPERTY(EditAnywhere, Category = "AI")
	float RotationInterpSpeed = 5.f;
};
