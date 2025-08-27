// Fill out your copyright notice in the Description page of Project Settings.


#include "Hamkt/AnimNotify/BossLeftArmSweepNotifyState.h"

void UBossLeftArmSweepNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	// 1. ���� ���۽� �̹� ���� ���� ��� �ʱ�ȭ (ƽ���� ��ȯ)
	AlreadyHitActors.Reset();

	// 2. �� �� ���� ���� �ձ�
	ArmSegments.Empty();
	ArmSegments.Add({ Socket_L_UpperArm, Socket_L_ForeArm });
	ArmSegments.Add({ Socket_L_ForeArm, Socket_L_Hand });
	ArmSegments.Add({ Socket_L_Hand,     Socket_L_Finger });
}
