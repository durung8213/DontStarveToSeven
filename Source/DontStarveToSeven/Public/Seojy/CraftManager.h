// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Containers/Map.h"
#include "Seojy/GridManager.h"
#include "Landscape.h"
#include "Kimmh/BasePlayerCharacter.h"
#include "Kimmh/Character/Controller/DTS_GamePlayPlayerController.h"
#include "GameFramework/Actor.h"
#include "Seojy/Struct/BuildingData.h"
#include "NavMesh/NavMeshBoundsVolume.h"	
#include "CraftManager.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Blueprintable)
class DONTSTARVETOSEVEN_API UCraftManager : public UActorComponent
{
	GENERATED_BODY()

public:

	// 생성자
	UCraftManager(const FObjectInitializer& ObjectInitializer);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// UI에서 들어온 건물 타입에 따라 고스트 메쉬 생성
	UFUNCTION()
	void SetGhostMesh(ECraftType CraftType);

	UFUNCTION(BlueprintCallable, Category = "Craft")
	void StartCraftMode(ECraftType craftType);

	UFUNCTION(BlueprintCallable, Category = "Craft")
	void EndCraftMode();

	UFUNCTION(Client, Reliable)
	void ClientEndCraftMode();


	/* 격자 시스템 */

	// 건물 배치 시도 함수
	UFUNCTION(BlueprintCallable, Category = "Craft")
	void TryPlaceBuilding();

	// 서버에서 건물 배치
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerTryPlaceBuilding(ECraftType CraftType, bool bCanBuild, int32 Width, 
								int32 Height, FRotator Rotation, FVector PlacementLocation);

	// 미리보기 함수 업데이트
	UFUNCTION(BlueprintCallable, Category = "Craft")
	void UpdateGhostMesh();

	//플레이어의 건물 배치 위치를 가져오는 함수 ( 라인 트레이스의 결과 )
	UFUNCTION()
	FVector GetPlacementLocation();

	// 우측 회전
	UFUNCTION(BlueprintCallable, Category = "Craft")
	void RotateRightGhostMesh();

	//좌측 회전
	UFUNCTION(BlueprintCallable, Category = "Craft")
	void RotateLeftGhostMesh();


protected:
	virtual void BeginPlay() override;

private:

	UFUNCTION(BlueprintPure, Category = "Craft")
	TSubclassOf<AActor> GetActorClass(ECraftType craftType);

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ABasePlayerCharacter* MyPlayer = Cast<ABasePlayerCharacter>(GetOwner());
	
	//GhostMesh : 건물 배치 미리보기를 위한 액터
	UPROPERTY()
	ABaseCraft* GhostMesh = nullptr;

	// 크래프트 배열
	UPROPERTY(EditAnywhere, Category = "CraftType")
	TArray<TSubclassOf<ABaseCraft>> CraftTypeArr;

private:

	UPROPERTY()
	ABasePlayerCharacter* OwnerPlayer;

	UPROPERTY()
	ADTS_GamePlayPlayerController* PlayerPC = nullptr;


	/* 격자 시스템 */
	UPROPERTY(VisibleAnywhere)
	UGridManager* GridManager = nullptr;

	UPROPERTY()
	TArray<AActor*> NavVolume;


	// 타이머 핸들
	UPROPERTY()
	FTimerHandle TimerHandle_UpdateGhostMesh;


};
