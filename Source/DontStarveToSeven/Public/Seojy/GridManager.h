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

	//���� �߽� ���� ��ǥ (�ͷ��� ���� ���� ����)
	UPROPERTY()
	FVector WorldLocation;

	// �� ���� ���� ( ���� ���� ���� )
	UPROPERTY()
	bool bIsOccupied;

	// ���� ����. (LineTrace�� ���� Z ��)
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

	// �ͷ��� �������� ���� �ʱ�ȭ
	void InitializeGridFromTerrain(AActor* TerrainActor, int32 DesiredCellSize);

	// ���� ��ǥ�� ���� ��ǥ�� �����ϰ�, �ش� ���� ���̸� �ݿ��Ͽ� ��ȯ
	FVector SnapToGrid(FVector WorldLocation);

	// SnapToGrid �����丵
	bool TrySnapToGrid(FVector WorldLocation, FVector& OutSnapedLocation);

	// �ش� ��ġ�� Width x Height ũ���� �ǹ��� ��ġ�� �� �ִ��� Ȯ��.
	bool CanPlaceBuilding(FVector Location, int32 Width, int32 Height);

	// �ش� ������ ���� ������ ���� Ȥ�� ���� 
	void MarkCellsOccupied(FVector Location, int32 Width, int32 Height, bool bOccupied);

	// ���� ��ǥ(������)�� ���� �� ������ ������. ��ȯ���� nullptr �̸� ���� ����.
	FGridCell* GetCellAtGridCoords(FIntPoint GridCoords);

	// Ư�� ���� ��ġ���� ���� ���̸� ���� 
	float GetTerrainHeight(FVector WorldLocation);

	// ��ü ���� ������ �����´�.
	const TMap<FIntPoint, FGridCell>& GetGridCells() const { return GridCells; }

	// ���� ��� ����� �Լ�
	void DrawDebugGrid(UWorld* World);

	// ���� �׸��� ��Į ����
	UFUNCTION(BlueprintCallable, Category = "Grid")
	void CreateGridDecals(UWorld* InWorld, UMaterialInterface* DecalMaterial);

	// �߰� : �ܺο��� ���� �����͸� �����ϱ� ���� �Լ�
	void SetWorld(UWorld* InWorld);

	// CachedWorld �� �̿��Ͽ� GetWorld()�� �������̵�
	virtual UWorld* GetWorld() const override; 
	
	// ��Į�� ����� �Լ�
	UFUNCTION(BlueprintCallable, Category = "Grid")
	void RemoveGridDecals();

public:
	//���� �⺻ ����
	
	UPROPERTY()
	int32 GridSize;		// �� �ϳ��� ũ��
	
	UPROPERTY()
	int32 GridCount = 0;	// ���� ����(����, ���� �����ϰ�)
	
	UPROPERTY()
	FVector GridOrigin;	// ���� ���� ���� ��ǥ


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid")
	TArray<UDecalComponent*> GridDecals;

	// 2���� ���� �����͸� TMap (Key : FIntPoint, Value : FGridCell)
	// ���� ��ü ���� ����ִ� ��ü
	UPROPERTY()
	TMap<FIntPoint, FGridCell> GridCells;


private:
	// �߰� : ���� ������ ĳ��
	UWorld* CachedWorld = nullptr;
	

};
