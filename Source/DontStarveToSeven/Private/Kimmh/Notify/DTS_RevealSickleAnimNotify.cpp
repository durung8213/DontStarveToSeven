// Fill out your copyright notice in the Description page of Project Settings.


#include "Kimmh/Notify/DTS_RevealSickleAnimNotify.h"
#include "Kimmh/BasePlayerCharacter.h"

void UDTS_RevealSickleAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner()) {
		ABasePlayerCharacter* CurrentPlayer = Cast<ABasePlayerCharacter>(MeshComp->GetOwner());
		if (CurrentPlayer) {
			CurrentPlayer->SetToolVisibility(ETool::Sickle, bRevealSickle);
		}
	}
}
