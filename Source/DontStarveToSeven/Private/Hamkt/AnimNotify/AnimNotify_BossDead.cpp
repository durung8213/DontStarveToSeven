// Fill out your copyright notice in the Description page of Project Settings.


#include "Hamkt/AnimNotify/AnimNotify_BossDead.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Hamkt/Enemy/EnemyBoss/EnemyBossBase.h"

void UAnimNotify_BossDead::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	// �Ͻ�����
	if (MeshComp)
		MeshComp->GlobalAnimRateScale = 0.f;
}