// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_BossReset.generated.h"

/**
 *  ���� HP Full, ������ �Ұ� true�� ���� , �����̺��Ʈ�� �ߴ�,���� bossreset ������ x�� ����
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
