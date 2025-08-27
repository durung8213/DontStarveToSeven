#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Leesh/SHPlayerController.h"
#include "Leesh/SHTotalStruct.h"
#include "SHGameModeBase.generated.h"


/**
 * 
 */

UCLASS()
class DONTSTARVETOSEVEN_API ASHGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASHGameModeBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	virtual void PostLogin(APlayerController* NewPlayer) override;


public:
	//캐릭터 세팅용 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Init")
	TArray<class APlayerStart*> PlayerStarts;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Init")
	int32 PlayerStartsIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Init")
	TSubclassOf<ABasePlayerCharacter> PlayerCharacterClass;
		
		
	/*UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	TSubclassOf<ABasePlayerCharacter> PlayerCharacterClass;*/


public:
	// 데이터 관리용 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataManage")
	TMap<FString, FPlayerInfo> PlayerInfoMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataManage")
	TMap<FGuid, FBuildingInfo> BuildingInfoMap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataManage")
	TMap<FGuid, FNatureInfo> NatureInfoMap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataManage")
	FTotalData TotalDataPack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataManage")
	UDataTable* CraftSpawnTable; // 크래프트 데이터테이블 할당.




public:
	// 데이터 관리용 함수
	UFUNCTION(BlueprintCallable, Category = "DataManage")
	void UpdatePlayerInfo(const FString& InNickName, const FCharacterStat& InCharacterStat, const TArray<FInventory>& InInventory, const TArray<FInventory>& InQuickSlot);
	UFUNCTION(BlueprintCallable, Category = "DataManage")
	void UpdateBuildingInfo(const FGuid& InGuid, const FString& InBuildingClassName, const float& InBuildingHP, const FTransform& InBuildingTransform);
	UFUNCTION(BlueprintCallable, Category = "DataManage")
	void UpdateNatureInfo(const FGuid& InGuid, const FString& InNatureClassName, const float& InNatureHP, const FTransform& InNatureTransform);
	
	UFUNCTION(BlueprintCallable, Category = "DataManage")
	void UpdateTotalDataPack();
	UFUNCTION(BlueprintCallable, Category = "DataManage")
	void SaveGameData();
	UFUNCTION(BlueprintCallable, Category = "DataManage")
	void LoadGameData();

	void SpawnCraft();

};	
