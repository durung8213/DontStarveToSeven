// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_StopAttackingIfTable.generated.h"

/**
 * 
 */
UCLASS()
class DONTSTARVETOSEVEN_API UBTS_StopAttackingIfTable : public UBTService
{
	GENERATED_BODY()

public:
	UBTS_StopAttackingIfTable();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;


protected:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Keys")
	FBlackboardKeySelector AttackTargetKey;

private:
	
};
