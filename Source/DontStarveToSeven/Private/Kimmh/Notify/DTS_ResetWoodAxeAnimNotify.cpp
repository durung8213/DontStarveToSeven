// Fill out your copyright notice in the Description page of Project Settings.


#include "Kimmh/Notify/DTS_ResetWoodAxeAnimNotify.h"
#include "Kimmh/BasePlayerCharacter.h"

void UDTS_ResetWoodAxeAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner()) {
		ABasePlayerCharacter* CurrentPlayer = Cast<ABasePlayerCharacter>(MeshComp->GetOwner());
		if (CurrentPlayer) {
			CurrentPlayer->SetToolVisibility(ETool::WoodAxe, false);
		}
	}
}
