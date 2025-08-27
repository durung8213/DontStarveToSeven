// Fill out your copyright notice in the Description page of Project Settings.


#include "Kimmh/NotifyState/DTS_NextComboAnimNotifyState.h"
#include "Kimmh/BasePlayerCharacter.h"
#include "Kismet/KismetSystemLibrary.h"

void UDTS_NextComboAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp && MeshComp->GetOwner()) {
		ABasePlayerCharacter* CurrentPlayer = Cast<ABasePlayerCharacter>(MeshComp->GetOwner());
		if (CurrentPlayer) {
			if (CurrentPlayer->bNormalAttackRequest) {
				// 다음 강공격으로 이동
				UKismetSystemLibrary::PrintString(this, NextNormalAttackComboSessionName.ToString());
				CurrentPlayer->PlayMontageWithSession(CurrentPlayer->MontageArr[MontageIndex], NextNormalAttackComboSessionName, NormalAttackPlayRate);
				CurrentPlayer->ResetAttackRequest();
				CurrentPlayer->ComboCount++;
			}
			else if (CurrentPlayer->bStrongAttackRequest) {
				// 다음 강공격으로 이동
				UKismetSystemLibrary::PrintString(this, NextStrongAttackComboSessionName.ToString());
				CurrentPlayer->PlayMontageWithSession(CurrentPlayer->MontageArr[MontageIndex], NextStrongAttackComboSessionName, StrongAttackPlayRate);
				CurrentPlayer->ResetAttackRequest();
				CurrentPlayer->ComboCount++;
			}
		}
	}
}

void UDTS_NextComboAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	if (MeshComp && MeshComp->GetOwner()) {
		ABasePlayerCharacter* CurrentPlayer = Cast<ABasePlayerCharacter>(MeshComp->GetOwner());
		if (CurrentPlayer) {
			if (CurrentPlayer->bNormalAttackRequest) {
				// 다음 강공격으로 이동
				UKismetSystemLibrary::PrintString(this, NextNormalAttackComboSessionName.ToString());
				CurrentPlayer->PlayMontageWithSession(CurrentPlayer->MontageArr[MontageIndex], NextNormalAttackComboSessionName, NormalAttackPlayRate);
				CurrentPlayer->ResetAttackRequest();
				CurrentPlayer->ComboCount++;
			}
			else if (CurrentPlayer->bStrongAttackRequest) {
				// 다음 강공격으로 이동
				UKismetSystemLibrary::PrintString(this, NextStrongAttackComboSessionName.ToString());
				CurrentPlayer->PlayMontageWithSession(CurrentPlayer->MontageArr[MontageIndex], NextStrongAttackComboSessionName, StrongAttackPlayRate);
				CurrentPlayer->ResetAttackRequest();
				CurrentPlayer->ComboCount++;
			}
		}
	}
}

void UDTS_NextComboAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner()) {
		ABasePlayerCharacter* CurrentPlayer = Cast<ABasePlayerCharacter>(MeshComp->GetOwner());
		if (CurrentPlayer) {
			if (CurrentPlayer->bNormalAttackRequest) {
				// 다음 강공격으로 이동
				UKismetSystemLibrary::PrintString(this, NextNormalAttackComboSessionName.ToString());
				CurrentPlayer->PlayMontageWithSession(CurrentPlayer->MontageArr[MontageIndex], NextNormalAttackComboSessionName, NormalAttackPlayRate);
				CurrentPlayer->ResetAttackRequest();
				CurrentPlayer->ComboCount++;
			}
			else if (CurrentPlayer->bStrongAttackRequest) {
				// 다음 강공격으로 이동
				UKismetSystemLibrary::PrintString(this, NextStrongAttackComboSessionName.ToString());
				CurrentPlayer->PlayMontageWithSession(CurrentPlayer->MontageArr[MontageIndex], NextStrongAttackComboSessionName, StrongAttackPlayRate);
				CurrentPlayer->ResetAttackRequest();
				CurrentPlayer->ComboCount++;
			}
		}
	}
}
