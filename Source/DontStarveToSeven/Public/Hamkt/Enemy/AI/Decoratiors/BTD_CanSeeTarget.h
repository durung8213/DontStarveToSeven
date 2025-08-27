// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_CanSeeTarget.generated.h"

/**
 * 
 */
UCLASS()
class DONTSTARVETOSEVEN_API UBTD_CanSeeTarget : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTD_CanSeeTarget();

protected:
	/* 블루프린트의 Perform the condition check */
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

private:
	UPROPERTY(EditAnywhere, Category = "Keys", meta = (AllowPrivateAccess = "true"))
	FBlackboardKeySelector AttackTargetKey;

};
