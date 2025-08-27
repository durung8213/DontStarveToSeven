// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIC_EnemyBossBase.generated.h"

/**
 * 
 */
class ABossZoneBase;
class AEnemyBossBase;
UCLASS()
class DONTSTARVETOSEVEN_API AAIC_EnemyBossBase : public AAIController
{
	GENERATED_BODY()

public:
	AAIC_EnemyBossBase();

	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION()
	void SelectAttackTarget(const FName& SelectTarget);
protected:

	// 비헤이비어트리 에셋 할당할 변수 , 에디터에서 설정
	UPROPERTY(EditDefaultsOnly, Category = "AI" )
	UBehaviorTree* BehaviorTreeAsset;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBlackboardComponent* BlackboardComp;

	UPROPERTY(EditAnywhere, Category = "AI")
	AEnemyBossBase* EnemyBossBase;
	
private:
	// AIC에서만 실행
	UFUNCTION()
	void HandleWakeUp();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerArea", meta = (AllowPrivateAccess = "true"))
	ABossZoneBase* BossZoneBase;

};
