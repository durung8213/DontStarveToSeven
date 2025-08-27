// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_CheckAttackTarget.generated.h"

/**
 * 
 */
UCLASS()
class DONTSTARVETOSEVEN_API UBTService_CheckAttackTarget : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
    UBTService_CheckAttackTarget();

protected:
    // �� ƽ(�Ǵ� �ֱ⸶��) ȣ��Ǵ� �Լ�
    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    // �˻� ����(��)
    UPROPERTY(EditAnywhere, Category = "Attack Target")
    float CheckInterval;
	
};
