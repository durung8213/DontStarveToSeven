// Fill out your copyright notice in the Description page of Project Settings.


#include "Kimmh/Notify/DTS_AttackStartAnimNotify.h"
#include "Kimmh/BasePlayerCharacter.h"
#include "Leesh/SHWeaponItem.h"

void UDTS_AttackStartAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner()) {
		ABasePlayerCharacter* CurrentPlayer = Cast<ABasePlayerCharacter>(MeshComp->GetOwner());
		if (CurrentPlayer) {
			ASHWeaponItem* CurrentWeapon = Cast<ASHWeaponItem>(CurrentPlayer->CurrentEquipItem);
			if (CurrentWeapon) {
				CurrentWeapon->AttackStart();
			}
		}
	}
}
