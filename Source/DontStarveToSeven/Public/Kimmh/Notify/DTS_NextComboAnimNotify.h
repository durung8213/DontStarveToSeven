// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "DTS_NextComboAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class DONTSTARVETOSEVEN_API UDTS_NextComboAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MontageIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName NextNormalAttackComboSessionName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float NormalAttackPlayRate = 1.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName NextStrongAttackComboSessionName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StrongAttackPlayRate = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName AttackEndSessionName;
};
