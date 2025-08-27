// Fill out your copyright notice in the Description page of Project Settings.


#include "Kimmh/Notify/DTS_PickUpItemAnimNotify.h"
#include "Kimmh/BasePlayerCharacter.h"
#include "Kimmh/Character/Controller/DTS_GamePlayPlayerController.h"
#include "Leesh/SHBaseItem.h"

void UDTS_PickUpItemAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	ABasePlayerCharacter* CurrentPlayer = Cast<ABasePlayerCharacter>(MeshComp->GetOwner());
	if (CurrentPlayer) {
		ADTS_GamePlayPlayerController* CurrentPlayerController = Cast<ADTS_GamePlayPlayerController>(CurrentPlayer->GetController());	// 플레이어 컨트롤러로 변환
		if (CurrentPlayer->CurrentItem && CurrentPlayerController) {
			CurrentPlayerController->AddItemToInventoryByName(CurrentPlayer->CurrentItem->ItemName, CurrentPlayer->CurrentItem->ItemCount);
			CurrentPlayer->ServerDestory(CurrentPlayer->CurrentItem);
			CurrentPlayer->CurrentItem = nullptr;
		}
	}

}
