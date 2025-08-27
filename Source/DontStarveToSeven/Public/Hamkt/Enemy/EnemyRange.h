// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hamkt/Enemy/EnemyBase.h"
#include "EnemyRange.generated.h"

/**
 * 
 */
UCLASS()
class DONTSTARVETOSEVEN_API AEnemyRange : public AEnemyBase
{
	GENERATED_BODY()

public:
	AEnemyRange();


	virtual void BeginPlay() override;

	virtual void SetMovementSpeed_Implementation(E_MovementSpeed NewSpeed) override;

	virtual void GetIdealRange_Implementation(float& AttackRadius, float& DefendRadius) const override;

	virtual void Attack_Implementation(FName NotifyName) override;

	virtual void ThrowWeaponAtTarget_Implementation(AActor* InWeapon, AActor* InTarget) override;

	virtual void ServerAttackMontage_Implementation(const FName& NotifyName) override;
	virtual void MulticastAttackMontage_Implementation(const FName& NotifyName) override;

	virtual void ServerThrowWeaponAtTarget_Implementation(AActor* InWeapon, AActor* InTarget) override;
	virtual void MulticastThrowWeaponAtTarget_Implementation(AActor* InWeapon, AActor* InTarget) override;
	
private:


};
