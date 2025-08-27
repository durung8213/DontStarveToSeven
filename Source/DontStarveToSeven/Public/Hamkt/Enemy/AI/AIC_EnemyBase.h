// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Hamkt/Enemy/AI/Enums/E_AISense.h"
#include "AIC_EnemyBase.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;
class UAISenseConfig_Damage;

/**
 * 
 */
UCLASS()
class DONTSTARVETOSEVEN_API AAIC_EnemyBase : public AAIController
{
	GENERATED_BODY()
	
public:
	AAIC_EnemyBase();

	// BeginPlay ���� �� ������ �����.
	virtual void OnPossess(APawn* InPawn) override;

	// ������ �Ǿ��� �� �ߵ�. ������ ����� ��ȯ
	UFUNCTION()
	void OnPerceptionUpdate(const TArray<AActor*>& UpdatedActors);

	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetStateAsPassive();

	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetStateAsInvestigating(FVector& Location);

	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetStateAsAttacking(AActor* AttackTarget);

	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetStateAsDead();


	// � ���� �����Ǿ����� ���ؼ� �˷��ִ� �Լ�
	UFUNCTION(BlueprintCallable, Category = "AI Perception")
	bool CanSenseActor(AActor* Actor, E_AISense Sense, FAIStimulus& OutStimulus);

	// ����� ���� ������ �Լ�
	TSubclassOf<class UAISense> ConvertSenseToClass(E_AISense Sense);

	UFUNCTION(BlueprintCallable, Category = "AI")
	E_AIState GetCurrentState();

	UFUNCTION(BlueprintCallable, Category = "AI Perception")
	void HandleSensedSight(AActor* AttackTarget);

	UFUNCTION(BlueprintCallable, Category = "GetterFunction")
	AActor* GetAttackTargetActor();

	UFUNCTION(BlueprintCallable)
	void OnAttackTargetDestroyed(AActor* DestroyedActor);

	

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Perception", meta = (AllowPrivateAccess = "true"))
	UAIPerceptionComponent* AIPerceptionComp;

	// Perception �þ� ���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Perception", meta = (AllowPrivateAccess = "true"))
	UAISenseConfig_Sight* SightConfig;

private:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BehaviorTreeAsset;

	// meta = (AllowPrivateAccess = "true"): private �����ӿ��� Blueprint���� ������ �� �ְ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackBoard Comp", meta = (AllowPrivateAccess = "true"))
	UBlackboardComponent* BlackboardComp;

	UPROPERTY(EditAnywhere, Category = "AttackTarget")
	AActor* AttackTargetActor;

};
