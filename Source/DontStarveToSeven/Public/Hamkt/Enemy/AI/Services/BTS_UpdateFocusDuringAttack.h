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
	// 블랙보드에 저장된 공격타겟 키
	UPROPERTY(EditAnywhere, Category = "Keys")
	FBlackboardKeySelector AttackTargetKey;

	// 회전 보간속도 ( 높을 수록 빠르게 회전함)
	UPROPERTY(EditAnywhere, Category = "AI")
	float RotationInterpSpeed = 5.f;
};
