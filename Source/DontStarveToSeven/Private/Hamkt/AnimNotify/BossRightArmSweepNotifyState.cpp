
// Fill out your copyright notice in the Description page of Project Settings.


#include "Hamkt/AnimNotify/BossRightArmSweepNotifyState.h"

void UBossRightArmSweepNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	// 1. ���� ���۽� �̹� ���� ���� ��� �ʱ�ȭ (ƽ���� ��ȯ)
	AlreadyHitActors.Reset();

	// 2. �� �� ���� ���� �ձ�
	ArmSegments.Add({ Socket_R_UpperArm, Socket_R_ForeArm });
	ArmSegments.Add({ Socket_R_ForeArm,  Socket_R_Hand });
	ArmSegments.Add({ Socket_R_Hand,     Socket_R_Finger });
}
