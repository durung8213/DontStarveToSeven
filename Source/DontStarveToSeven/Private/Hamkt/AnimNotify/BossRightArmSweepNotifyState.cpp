
// Fill out your copyright notice in the Description page of Project Settings.


#include "Hamkt/AnimNotify/BossRightArmSweepNotifyState.h"

void UBossRightArmSweepNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	// 1. 공격 시작시 이미 맞은 액터 목록 초기화 (틱마다 소환)
	AlreadyHitActors.Reset();

	// 2. 팔 각 구간 소켓 잇기
	ArmSegments.Add({ Socket_R_UpperArm, Socket_R_ForeArm });
	ArmSegments.Add({ Socket_R_ForeArm,  Socket_R_Hand });
	ArmSegments.Add({ Socket_R_Hand,     Socket_R_Finger });
}
