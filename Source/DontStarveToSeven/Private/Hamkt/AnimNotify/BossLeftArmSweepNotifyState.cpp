// Fill out your copyright notice in the Description page of Project Settings.


#include "Hamkt/AnimNotify/BossLeftArmSweepNotifyState.h"

void UBossLeftArmSweepNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	// 1. 공격 시작시 이미 맞은 액터 목록 초기화 (틱마다 소환)
	AlreadyHitActors.Reset();

	// 2. 팔 각 구간 소켓 잇기
	ArmSegments.Empty();
	ArmSegments.Add({ Socket_L_UpperArm, Socket_L_ForeArm });
	ArmSegments.Add({ Socket_L_ForeArm, Socket_L_Hand });
	ArmSegments.Add({ Socket_L_Hand,     Socket_L_Finger });
}
