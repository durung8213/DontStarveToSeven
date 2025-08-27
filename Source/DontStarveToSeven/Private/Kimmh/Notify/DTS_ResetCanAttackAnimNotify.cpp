// Fill out your copyright notice in the Description page of Project Settings.


#include "Kimmh/Notify/DTS_ResetCanAttackAnimNotify.h"
#include "Kimmh/BasePlayerCharacter.h"

void UDTS_ResetCanAttackAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner()) {
		ABasePlayerCharacter* CurrentPlayer = Cast<ABasePlayerCharacter>(MeshComp->GetOwner());
		CurrentPlayer->ResetbCanAttack();
	}
}
