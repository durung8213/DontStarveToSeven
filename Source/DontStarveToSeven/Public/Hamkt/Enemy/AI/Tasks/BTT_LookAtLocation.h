// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_LookAtLocation.generated.h"

/**
 * 
 */
UCLASS()
class DONTSTARVETOSEVEN_API UBTT_LookAtLocation : public UBTTaskNode
{
	GENERATED_BODY()

public:
    UBTT_LookAtLocation();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector LookAtLocationKey;
	
};
