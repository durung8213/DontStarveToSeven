// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hamkt/AnimNotify/BossArmCapsuleSweepNotifyState.h"
#include "BossRightArmSweepNotifyState.generated.h"

// 팔의 각 구간을 스윕할 때 사용할 소켓 이름

static const FName Socket_R_UpperArm(TEXT("socket_r_upperarm"));
static const FName Socket_R_ForeArm(TEXT("socket_r_forearm"));
static const FName Socket_R_Hand(TEXT("socket_r_hand"));
static const FName Socket_R_Finger(TEXT("socket_r_finger"));

/**
 * 
 */

UCLASS()
class DONTSTARVETOSEVEN_API UBossRightArmSweepNotifyState : public UBossArmCapsuleSweepNotifyState
{
	GENERATED_BODY()
	
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
};
