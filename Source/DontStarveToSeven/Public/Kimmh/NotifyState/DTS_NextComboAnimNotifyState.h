// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "DTS_NextComboAnimNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class DONTSTARVETOSEVEN_API UDTS_NextComboAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

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
};
