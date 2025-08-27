// Fill out your copyright notice in the Description page of Project Settings.


#include "Kimmh/Notify/DTS_ResetSickleAnimNotify.h"
#include "Kimmh/BasePlayerCharacter.h"
void UDTS_ResetSickleAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner()) {
		ABasePlayerCharacter* CurrentPlayer = Cast<ABasePlayerCharacter>(MeshComp->GetOwner());
		if (CurrentPlayer) {
			CurrentPlayer->SetToolVisibility(ETool::Sickle, false);
		}
	}
}
