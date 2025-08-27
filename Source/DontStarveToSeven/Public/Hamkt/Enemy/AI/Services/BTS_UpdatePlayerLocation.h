// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_UpdatePlayerLocation.generated.h"


/**
 * 
 */
UCLASS()
class DONTSTARVETOSEVEN_API UBTS_UpdatePlayerLocation : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTS_UpdatePlayerLocation();

protected:
	// �� ƽ���� ȣ��� (�����̺��Ʈ������ ���ݼ��� ����)
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, Category = "Keys")
	FBlackboardKeySelector PlayerLocationKey;

};
