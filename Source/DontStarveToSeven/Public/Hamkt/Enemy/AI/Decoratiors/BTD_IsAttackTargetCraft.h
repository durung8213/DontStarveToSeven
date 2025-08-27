// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTD_IsAttackTargetCraft.generated.h"

/**
 * 
 */
UCLASS()
class DONTSTARVETOSEVEN_API UBTD_IsAttackTargetCraft : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

public:

	UBTD_IsAttackTargetCraft();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
};
