// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hamkt/Enemy/EnemyBase.h"
#include "EnemyMelee.generated.h"

/**
 * 
 */
UCLASS()
class DONTSTARVETOSEVEN_API AEnemyMelee : public AEnemyBase
{
	GENERATED_BODY()

public:
	AEnemyMelee();

	virtual void BeginPlay() override;
	virtual void Attack_Implementation(FName NotifyName) override;

	virtual void ServerAttackMontage_Implementation(const FName& NotifyName) override;
	virtual void MulticastAttackMontage_Implementation(const FName& NotifyName) override;
	
protected:

};
