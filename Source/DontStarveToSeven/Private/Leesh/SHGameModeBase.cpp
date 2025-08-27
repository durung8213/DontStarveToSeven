// Fill out your copyright notice in the Description page of Project Settings.


#include "Leesh/SHGameModeBase.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Leesh/Save/DTS_SaveGame.h"
#include "Kimmh/BasePlayerCharacter.h"
#include "Kimmh/Character/PlayerState/DTS_BasePlayerState.h"
#include "Kimmh/Character/Controller/DTS_GamePlayPlayerController.h"



ASHGameModeBase::ASHGameModeBase()
{
	//PlayerControllerClass = ASHPlayerController::StaticClass();

	static ConstructorHelpers::FClassFinder<ABasePlayerCharacter> PlayerCharacterBPClass(TEXT("/Game/Kimmh/Character/BP_TestBasePlayerCharacter.BP_TestBasePlayerCharacter_C"));
	if (PlayerCharacterBPClass.Succeeded())
	{
		PlayerCharacterClass = PlayerCharacterBPClass.Class;
	}

}

void ASHGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("SHGameStart"));

	// ��� PlayerStart ���� ã��
	TArray<AActor*> TempActors;
	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), TempActors);

	for (AActor* Actor : TempActors)
	{
		if (APlayerStart* Start = Cast<APlayerStart>(Actor))
		{
			PlayerStarts.Add(Start);
		}
	}


	CraftSpawnTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Leesh/Table/DT_CraftSpawn.DT_CraftSpawn"));	// ������ ���̺� �ҷ���


	SpawnCraft();


}

void ASHGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


AActor* ASHGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	if (PlayerStarts.Num() == 0)
	{
		return Super::ChoosePlayerStart_Implementation(Player); // ������ġ �������� �ʾ��� ��, ���� �⺻ �������� ����.
	}

	// ���� �ε����� �ش��ϴ� PlayerStart ��ȯ
	APlayerStart* ChosenStart = PlayerStarts[PlayerStartsIndex];

	// ���� �ε����� ��ȯ (0���� �ǵ��ƿ���)
	PlayerStartsIndex = (PlayerStartsIndex + 1) % PlayerStarts.Num();

	return ChosenStart;
}

void ASHGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	UE_LOG(LogTemp, Warning, TEXT("Player connection detection: %s"), *NewPlayer->GetName());

	FInputModeGameOnly InputMode;

	NewPlayer->SetInputMode(InputMode);
	NewPlayer->bShowMouseCursor = false;

	LoadGameData();

	// �α����� PC��, Ŀ���� PC Ÿ������ ĳ��Ʈ
	ADTS_GamePlayPlayerController* PC = Cast<ADTS_GamePlayPlayerController>(NewPlayer);
	if(!PC || PlayerStarts.Num() == 0)
	{
		return;
	}

	PC->ClientRequestNickname();



	// ���� ��ġ ����
	AActor* ChosenStart = ChoosePlayerStart(NewPlayer);
	if (!ChosenStart)
	{
		return;
	}

	// Ʈ������ ����
	const FTransform SpawnTransform = ChosenStart->GetActorTransform();

	// ĳ���� ���� �� ������
	ABasePlayerCharacter* NewCharacter = GetWorld()->SpawnActor<ABasePlayerCharacter>(PlayerCharacterClass, SpawnTransform);
	if (NewCharacter && NewPlayer)
	{
		if (APawn* PawnToPossess = Cast<APawn>(NewCharacter))
		{
			NewPlayer->Possess(PawnToPossess);
			UE_LOG(LogTemp, Warning, TEXT("Character Possess: %s"), *PawnToPossess->GetName());
		}
	}


	// ��Ʈ�ѷ��� ĳ�����Ͽ� Ŀ���� �Լ� ȣ��
	if (PC)
	{
		const FString OnwerNickname = PC->NickName;

		if (PlayerInfoMap.Contains(OnwerNickname))
		{
			// ���� ����
			ADTS_BasePlayerState* MyPlayerState = NewPlayer->GetPlayerState<ADTS_BasePlayerState>();
			FPlayerInfo* FoundInfo = PlayerInfoMap.Find(OnwerNickname);
			MyPlayerState->SetAllPlayerStat(FoundInfo->Stat);
			PC->InventoryComponent->Inventory = FoundInfo->Inven;
			PC->InventoryComponent->QuickSlot = FoundInfo->Quick;
			UE_LOG(LogTemp, Warning, TEXT("This is an existing user."))
		}
		else
		{
			// �ű� ����
			UE_LOG(LogTemp, Warning, TEXT("You are a new user."))

		}

	}

}





void ASHGameModeBase::UpdatePlayerInfo(const FString& InNickName, const FCharacterStat& InCharacterStat, const TArray<FInventory>& InInventory, const TArray<FInventory>& InQuickSlot)
{
	// ���� �� ����ü�� ����.
	FPlayerInfo TempStruct;
	TempStruct.Stat = InCharacterStat;
	TempStruct.Inven = InInventory;
	TempStruct.Quick = InQuickSlot;
	
	// �� �����̳ʿ� �߰�.
	PlayerInfoMap.Add(InNickName, TempStruct);

	UpdateTotalDataPack();

	// �ʿ� �߰��� �� �����
	FPlayerInfo* FoundValue = PlayerInfoMap.Find(InNickName);
	FInventory* FoundItem = FoundValue->Inven.FindByPredicate([&](const FInventory& Item)
	{
			return Item.ItemName == TEXT("TestItem");
	});


	UE_LOG(LogTemp, Log, TEXT("GameMode: Received Player Info from %s"), *InNickName);
	UE_LOG(LogTemp, Log, TEXT("GameMode: Received Player Info ItemName : %s"), *FoundItem->ItemName);

}

void ASHGameModeBase::UpdateBuildingInfo(const FGuid& InGuid, const FString& InBuildingClassName, const float& InBuildingHP, const FTransform& InBuildingTransform)
{
	// ���� �� ����ü�� ����.
	FBuildingInfo TempStruct;
	TempStruct.BuildingClassName = InBuildingClassName;
	TempStruct.BuildingHP = InBuildingHP;
	TempStruct.BuildingTransform = InBuildingTransform;

	// �� �����̳ʿ� �߰�.
	BuildingInfoMap.Add(InGuid, TempStruct);

	UpdateTotalDataPack();

	// �ʿ� �߰��� �� �����
	FBuildingInfo* FoundValue = BuildingInfoMap.Find(InGuid);
	FString FoundName = FoundValue->BuildingClassName;

	UE_LOG(LogTemp, Log, TEXT("GameMode: Received Building Info from %s"), *InGuid.ToString());
	UE_LOG(LogTemp, Log, TEXT("GameMode: Received Building Info ItemName : %s"), *FoundName);


}

void ASHGameModeBase::UpdateNatureInfo(const FGuid& InGuid, const FString& InNatureClassName, const float& InNatureHP, const FTransform& InNatureTransform)
{
	// ���� �� ����ü�� ����.
	FNatureInfo TempStruct;
	TempStruct.NatureClassName = InNatureClassName;
	TempStruct.NatureHP = InNatureHP;
	TempStruct.NatureTransform = InNatureTransform;

	// �� �����̳ʿ� �߰�.
	NatureInfoMap.Add(InGuid, TempStruct);

	// �ʿ� �߰��� �� �����
	FNatureInfo* FoundValue = NatureInfoMap.Find(InGuid);
	FString FoundName = FoundValue->NatureClassName;

	UE_LOG(LogTemp, Log, TEXT("GameMode: Received Nature Info from %s"), *InGuid.ToString());
	UE_LOG(LogTemp, Log, TEXT("GameMode: Received Nature Info ItemName : %s"), *FoundName);

}

void ASHGameModeBase::UpdateTotalDataPack()
{
	FTotalData TempTotalDataPack;
	TempTotalDataPack.PlayerInfoMapPack = PlayerInfoMap;
	TempTotalDataPack.BuildingInfoMapPack = BuildingInfoMap;
	TempTotalDataPack.NatureInfoMapPack = NatureInfoMap;

	TotalDataPack = TempTotalDataPack;

	SaveGameData();
}

void ASHGameModeBase::SaveGameData()
{
	UDTS_SaveGame* SaveGameInstance = Cast<UDTS_SaveGame>(UGameplayStatics::CreateSaveGameObject(UDTS_SaveGame::StaticClass()));
	SaveGameInstance->SavedTotalDataPack = TotalDataPack;

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("TotalSaveSlot"), 0);
	UE_LOG(LogTemp, Warning, TEXT("Your game has been saved on the server."));
}

void ASHGameModeBase::LoadGameData()
{
	if(UGameplayStatics::DoesSaveGameExist(TEXT("TotalSaveSlot"), 0))
	{
		UDTS_SaveGame* LoadGameInstance = Cast<UDTS_SaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("TotalSaveSlot"), 0));

		if (LoadGameInstance)
		{
			// ��Ż ������ �ε�
			TotalDataPack = LoadGameInstance->SavedTotalDataPack;
			PlayerInfoMap = TotalDataPack.PlayerInfoMapPack;
			BuildingInfoMap = TotalDataPack.BuildingInfoMapPack;
			NatureInfoMap = TotalDataPack.NatureInfoMapPack;

			// ����׿� ����.
			for (const TPair<FString, FPlayerInfo>& Elem : PlayerInfoMap)
			{
				const FString& Nickname = Elem.Key;
				const FPlayerInfo& Info = Elem.Value;

				UE_LOG(LogTemp, Warning, TEXT("=== Player Info ==="));
				UE_LOG(LogTemp, Warning, TEXT("�г���: %s"), *Nickname);

				// 1. Stat ��� (��: HP, ���ݷ� ��)
				UE_LOG(LogTemp, Warning, TEXT("Stat - HP: %f, ATK: %f"), Info.Stat.CurrentHP, Info.Stat.Atk);

				// 2. �κ��丮 ���
				for (int32 i = 0; i < Info.Inven.Num(); ++i)
				{
					const FInventory& Item = Info.Inven[i];
					UE_LOG(LogTemp, Warning, TEXT("Inven[%d] - ItemID: %s, Count: %d"), i, *Item.ItemName, Item.ItemCount);
				}

				// 3. ������ ���
				for (int32 i = 0; i < Info.Quick.Num(); ++i)
				{
					const FInventory& Item = Info.Quick[i];
					UE_LOG(LogTemp, Warning, TEXT("Quick[%d] - ItemID: %s, Count: %d"), i, *Item.ItemName, Item.ItemCount);
				}
			}
		
		}

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("There is no saved data stored on the server. A new game has begun."));
	}
}

void ASHGameModeBase::SpawnCraft()
{
	if (BuildingInfoMap.Num() == 0 || !CraftSpawnTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("BuildingInfoMap is empty or CraftSpawnTable is null. Skipping spawn."));
		return;
	}

		
	TArray<FGuid> AllKeys;
	BuildingInfoMap.GetKeys(AllKeys);


	if (CraftSpawnTable)
	{
		for (const FGuid& Guid : AllKeys)
		{
			if (FBuildingInfo* Info = BuildingInfoMap.Find(Guid))
			{
				FName RowName = FName(*Info->BuildingClassName);
				const FString ContextString(TEXT("SpawnLookup"));

				FCraftRecipe* RowData = CraftSpawnTable->FindRow<FCraftRecipe>(RowName, ContextString);
				ABaseCraft* SpawnActor = GetWorld()->SpawnActor<ABaseCraft>
					(
						RowData->CraftClass,
						Info->BuildingTransform

					);
				SpawnActor->CraftHP = Info->BuildingHP;
				SpawnActor->CraftID = Guid;
				SpawnActor->SetCraftMode(ECraftMode::Actual);

					UE_LOG(LogTemp, Warning, TEXT("Found Building: %s"), *Info->BuildingClassName);
			}
		}
	}

}

