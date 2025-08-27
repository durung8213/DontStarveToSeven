// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "DTS_SickleCollisionOnAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class DONTSTARVETOSEVEN_API UDTS_SickleCollisionOnAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCollisionEnable = true;
};
