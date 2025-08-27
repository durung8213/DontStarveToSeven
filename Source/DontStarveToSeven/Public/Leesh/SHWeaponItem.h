// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Leesh/SHBaseItem.h"
#include "SHWeaponItem.generated.h"

/**
 * 
 */
UCLASS()
class DONTSTARVETOSEVEN_API ASHWeaponItem : public ASHBaseItem
{
	GENERATED_BODY()
	

public:
	// Sets default values for this actor's properties
	ASHWeaponItem();

	UFUNCTION()
	void OnCapsuleBeginOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	virtual void BeginPlay() override;

	void AttackStart();

	void AttackStop();
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UCapsuleComponent> HitCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> HitActorArr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanAttack = false;

	virtual void SetCollisionObjectTypeWeapon() override;
};
