// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFrameWork/Actor.h"
#include "Seojy/BaseCraft.h"
#include "Landscape.h"
#include "Components/DecalComponent.h"
#include "GridManager.generated.h"

USTRUCT()
struct FGridCell
{
	GENERATED_BODY()

	//셀의 중심 월드 좌표 (터레인 높이 보정 포함)
	UPROPERTY()
	FVector WorldLocation;

	// 셀 점유 상태 ( 현재 셀의 상태 )
	UPROPERTY()
	bool bIsOccupied;

	// 지형 높이. (LineTrace로 얻은 Z 값)
	UPROPERTY()
	float TerrainHeight;

	FGridCell()
		:WorldLocation(FVector::ZeroVector), 
		TerrainHeight(0.0f), 
		bIsOccupied(false)
	{

	}
};

UCLASS(Blueprintable)
class DONTSTARVETOSEVEN_API UGridManager : public UObject
{
	GENERATED_BODY()

public:

	// 터레인 기준으로 격자 초기화
	void InitializeGridFromTerrain(AActor* TerrainActor, int32 DesiredCellSize);

	// 월드 좌표를 격자 좌표로 스냅하고, 해당 셀의 높이를 반영하여 반환
	FVector SnapToGrid(FVector WorldLocation);

	// SnapToGrid 리팩토링
	bool TrySnapToGrid(FVector WorldLocation, FVector& OutSnapedLocation);

	// 해당 위치에 Width x Height 크기의 건물을 배치할 수 있는지 확인.
	bool CanPlaceBuilding(FVector Location, int32 Width, int32 Height);

	// 해당 영역의 격자 셀들을 점유 혹은 해제 
	void MarkCellsOccupied(FVector Location, int32 Width, int32 Height, bool bOccupied);

	// 격자 좌표(정수형)를 통해 셀 정보를 가져옴. 반환값이 nullptr 이면 영역 밖임.
	FGridCell* GetCellAtGridCoords(FIntPoint GridCoords);

	// 특정 월드 위치에서 지형 높이를 감지 
	float GetTerrainHeight(FVector WorldLocation);

	// 전체 격자 정보를 가져온다.
	const TMap<FIntPoint, FGridCell>& GetGridCells() const { return GridCells; }

	// 격자 모드 디버깅 함수
	void DrawDebugGrid(UWorld* World);

	// 격자 그리드 데칼 생성
	UFUNCTION(BlueprintCallable, Category = "Grid")
	void CreateGridDecals(UWorld* InWorld, UMaterialInterface* DecalMaterial);

	// 추가 : 외부에서 월드 포인터를 설정하기 위한 함수
	void SetWorld(UWorld* InWorld);

	// CachedWorld 를 이용하여 GetWorld()를 오버라이드
	virtual UWorld* GetWorld() const override; 
	
	// 데칼을 지우는 함수
	UFUNCTION(BlueprintCallable, Category = "Grid")
	void RemoveGridDecals();

public:
	//격자 기본 정보
	
	UPROPERTY()
	int32 GridSize;		// 셀 하나의 크기
	
	UPROPERTY()
	int32 GridCount = 0;	// 셀의 개수(가로, 세로 동일하게)
	
	UPROPERTY()
	FVector GridOrigin;	// 격자 시작 월드 좌표


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid")
	TArray<UDecalComponent*> GridDecals;

	// 2차원 격자 데이터를 TMap (Key : FIntPoint, Value : FGridCell)
	// 맵의 전체 셀을 담고있는 객체
	UPROPERTY()
	TMap<FIntPoint, FGridCell> GridCells;


private:
	// 추가 : 월드 포인터 캐시
	UWorld* CachedWorld = nullptr;
	

};
