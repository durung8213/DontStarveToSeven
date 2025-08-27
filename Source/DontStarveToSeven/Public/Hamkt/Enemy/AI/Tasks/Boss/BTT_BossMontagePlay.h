// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_BossMontagePlay.generated.h"

/**
 * 
 */
UCLASS()
class DONTSTARVETOSEVEN_API UBTT_BossMontagePlay : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_BossMontagePlay();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UFUNCTION()
	void OnMontageFinished(UAnimMontage* Montage, bool bInterrupted);

private:
	UPROPERTY()
	UBehaviorTreeComponent* CachedOwnerComp;
};
