// Fill out your copyright notice in the Description page of Project Settings.


#include "Kimmh/Notify/ShootNextAnimNotify.h"
#include "Kimmh/BasePlayerCharacter.h"
void UShootNextAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner()) {
		ABasePlayerCharacter* CurrentPlayer = Cast<ABasePlayerCharacter>(MeshComp->GetOwner());
		if (CurrentPlayer) {
			CurrentPlayer->ResetbIsAiming();
			CurrentPlayer->SetbOrientRotationToMovement(true);
			/*CurrentPlayer->bUseControllerRotationYaw = false;*/

		}
		//CurrentPlayer->GetCharacterMovement()->bOrientRotationToMovement = true;
		/*if (CurrentPlayer->bIsAiming) {
			CurrentPlayer->PlayMontageWithSession(CurrentPlayer->BowMontage, FName(TEXT("BowAfterAim")));
		}
		else {
			CurrentPlayer->PlayMontageWithSession(CurrentPlayer->BowMontage, FName(TEXT("BowAfterIdle")));
		}*/
	}
}
