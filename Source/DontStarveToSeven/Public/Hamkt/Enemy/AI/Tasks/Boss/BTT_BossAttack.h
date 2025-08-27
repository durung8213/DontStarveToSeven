// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_BossAttack.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EEnemyAttackType : uint8
{
	Attack1		UMETA(DisplayName = "Attack1"),
	Attack2		UMETA(DisplayName = "Attack2"),
	Attack3		UMETA(DisplayName = "Attack3"),
	Attack4		UMETA(DisplayName = "Attack4"),
	FWAttack1	UMETA(DisplayName = "FWAttack1"),
	FWAttack2	UMETA(DisplayName = "FWAttack2"),
	JumpAttack	UMETA(DisplayName = "JumpAttack"),
	BiteForward	UMETA(DisplayName = "BiteForward")
};

UCLASS()
class DONTSTARVETOSEVEN_API UBTT_BossAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_BossAttack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UFUNCTION()
	void OnAttackMontageFinished(UAnimMontage* Montage, bool bInterrupted);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	EEnemyAttackType AttackType;

	UFUNCTION()
	void DashAttack(UBlackboardComponent* InBlackboardComp, AEnemyBossBase* InEnemyBossPawn, float HorizontalSpeed, float VertialSpeed);

private:
	UPROPERTY()
	UBehaviorTreeComponent* CachedOwnerComp;

	
};
