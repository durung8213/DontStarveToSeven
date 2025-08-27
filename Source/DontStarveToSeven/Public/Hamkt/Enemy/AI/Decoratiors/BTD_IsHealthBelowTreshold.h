// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_IsHealthBelowTreshold.generated.h"

/**
 * 
 */
UCLASS()
class DONTSTARVETOSEVEN_API UBTD_IsHealthBelowTreshold : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTD_IsHealthBelowTreshold();

	virtual FString GetStaticDescription() const override;

	/* 블루프린트의 Perform the condition check */
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))	// 최소값, 최댓값 설정
	float HealthThreshold;
private:
	
};
