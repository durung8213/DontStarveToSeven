// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hamkt/AnimNotify/BossArmCapsuleSweepNotifyState.h"
#include "BossLeftArmSweepNotifyState.generated.h"

// 팔의 각 구간을 스윕할 때 사용할 소켓 이름
static const FName Socket_L_UpperArm(TEXT("socket_l_upperarm"));
static const FName Socket_L_ForeArm(TEXT("socket_l_forearm"));
static const FName Socket_L_Hand(TEXT("socket_l_hand"));
static const FName Socket_L_Finger(TEXT("socket_l_finger"));

/**
 * 
 */

UCLASS()
class DONTSTARVETOSEVEN_API UBossLeftArmSweepNotifyState : public UBossArmCapsuleSweepNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
protected:
	
};
