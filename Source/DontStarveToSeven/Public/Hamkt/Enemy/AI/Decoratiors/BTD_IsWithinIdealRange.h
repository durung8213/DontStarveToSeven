// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_IsWithinIdealRange.generated.h"

/**
 * 
 */
UCLASS()
class DONTSTARVETOSEVEN_API UBTD_IsWithinIdealRange : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTD_IsWithinIdealRange();
	
protected:
	/* 블루프린트의 Perform the condition check */
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

private:
	UPROPERTY(EditAnywhere, Category = "Keys", meta = (AllowPrivateAccess = "true"))
	FBlackboardKeySelector IdealRangeKey;

	UPROPERTY(EditAnywhere, Category = "Keys", meta = (AllowPrivateAccess = "true"))
	FBlackboardKeySelector AttackTargetKey;
};
