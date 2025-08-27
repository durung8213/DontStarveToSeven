// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_BossReset.generated.h"

/**
 *  보스 HP Full, 블랙보드 불값 true로 변경 , 비헤이비어트리 중단,시점 bossreset 액터의 x축 기준
 */
UCLASS()
class DONTSTARVETOSEVEN_API UBTT_BossReset : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_BossReset();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UBehaviorTreeComponent* CachedOwnerComp;
};
