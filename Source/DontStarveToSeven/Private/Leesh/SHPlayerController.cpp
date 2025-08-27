// Fill out your copyright notice in the Description page of Project Settings.

#include "Leesh/SHPlayerController.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Components/InputComponent.h"
#include "GameFramework/PlayerInput.h"
#include "GameFramework/PlayerState.h"
#include "Kimmh/Character/PlayerState/DTS_BasePlayerState.h"
#include "Leesh/SHGameModeBase.h"
#include "Net/UnrealNetwork.h"



ASHPlayerController::ASHPlayerController()
{
	InventoryComponent = CreateDefaultSubobject<USHInventoryComponent>(TEXT("InventoryComponent"));
}

void ASHPlayerController::BeginPlay()
{

}

void ASHPlayerController::SetupInputComponent()
{

	Super::SetupInputComponent();

	// 1번 키를 눌렀을 때 OnOneKeyPressed 함수 호출
	InputComponent->BindAction("OneKey", IE_Pressed, this, &ASHPlayerController::OnOneKeyPressed);

}








bool ASHPlayerController::OnOneKeyPressed_Validate()
{
	return true; // 기본적으로 항상 유효함
}

void ASHPlayerController::OnOneKeyPressed_Implementation()
{
	if (HasAuthority())  // 서버에서만 실행 확인
	{
		UE_LOG(LogTemp, Warning, TEXT("OnOneKeyPressed run at Server"));
		// 1번 키가 눌렸을 때 처리할 이벤트
		UE_LOG(LogTemp, Warning, TEXT("Num 1 Key Pressed"));
		USHInventoryComponent* InventoryComp = FindComponentByClass<USHInventoryComponent>();
		if (InventoryComp)
		{
			FInventory NewItem;
			NewItem.ItemName = TEXT("TestItem");
			NewItem.ItemCount = 1;
			NewItem.bUseCoord = false;
			InventoryComp->AddItem(NewItem, EInventoryType::Inventory);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UInventoryComponent not found on PlayerController"));
		}
	}

}


void ASHPlayerController::ClientRequestNickname_Implementation()
{
	GetNickName();
}

void ASHPlayerController::GetNickName()
{
	// GameInstance에서 닉네임 가져오기
	//if (UDTS_GameInstance* GI = Cast<UDTS_GameInstance>(GetGameInstance()))
	//{
	//	NickName = GI->NickName;
	//	ServerSetNickName(NickName); // 서버에서 Replicate 되도록 설정
	//}

}


bool ASHPlayerController::ServerSetNickName_Validate(const FString& NewNickName)
{
	return true;
}

void ASHPlayerController::ServerSetNickName_Implementation(const FString& NewNickName)
{
	NickName = NewNickName;
}


bool ASHPlayerController::ServerTossInfoToGameMode_Validate()
{
	return true;
}

void ASHPlayerController::ServerTossInfoToGameMode_Implementation()
{
	ASHGameModeBase* GM = Cast<ASHGameModeBase>(GetWorld()->GetAuthGameMode());
	if(GM)
	{
		ADTS_BasePlayerState* State = Cast<ADTS_BasePlayerState>(PlayerState);
		if(State)
		{
			UE_LOG(LogTemp, Warning, TEXT("Success to cast GM"));
			FCharacterStat TempStatStruct = State->GetAllPlayerStat();
			GM->UpdatePlayerInfo(NickName, TempStatStruct, InventoryComponent->Inventory, InventoryComponent->QuickSlot);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Fail to cast GM"));
		}
	}


}

void ASHPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASHPlayerController, NickName);
}