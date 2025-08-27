// Fill out your copyright notice in the Description page of Project Settings.


#include "Hamkt/Enemy/AI/Decoratiors/BTD_IsAttackTargetCraft.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Actor.h"

UBTD_IsAttackTargetCraft::UBTD_IsAttackTargetCraft()
{
	NodeName = TEXT("Is AttackTarget Craft");
}

bool UBTD_IsAttackTargetCraft::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!IsValid(BlackboardComp)) return false;

	UObject* TargetObject = BlackboardComp->GetValueAsObject(GetSelectedBlackboardKey());

	AActor* TargetActor = Cast<AActor>(TargetObject);

	// 유효한 액터이고, "Craft" 태그가 있으면 true
	if (TargetActor && TargetActor->ActorHasTag(FName("Craft")))
	{
		return true;
	}

	return false;
}
