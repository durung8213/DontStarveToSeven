//Fill out your copyright notice in the Description page of Project Settings.


#include "Seojy/GridManager.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Components/DecalComponent.h"
#include "Kismet/KismetSystemLibrary.h"


void UGridManager::SetWorld(UWorld* InWorld)
{
	CachedWorld = InWorld;
}

UWorld* UGridManager::GetWorld() const
{
	if (CachedWorld)
	{
		return CachedWorld;
	}
	
	if (GetOuter())
	{
		return GetOuter()->GetWorld();
	}
	return nullptr;
}

void UGridManager::RemoveGridDecals()
{
	// �迭�� ����� ��Į ������Ʈ�� �ִٸ� ��� �����Ѵ�.
	for (UDecalComponent* ExistingDecal : GridDecals)
	{
		if (ExistingDecal)
		{
			ExistingDecal->DestroyComponent();
		}
	}

	GridDecals.Empty();


}

FVector UGridManager::SnapToGrid(FVector WorldLocation)
{
	//���� ��ǥ�� ���� �ε����� ��ȯ
	// �ش� �����̼� ��ǥ�� ��� ��� ��ǥ�� ���Ѵ�.
	int32 GridX = FMath::RoundToInt((WorldLocation.X - GridOrigin.X) / GridSize);
	int32 GridY = FMath::RoundToInt((WorldLocation.Y - GridOrigin.Y) / GridSize);

	GridX = FMath::Clamp(GridX, 0, GridCount - 1);
	GridY = FMath::Clamp(GridY, 0, GridCount - 1);

	// ���� �� �ش� ���� ���� ���� ���
	FIntPoint Key(GridX, GridY);

	float Height = GridOrigin.Z;

	if (!GridCells.Contains(Key))
	{
		// �ش� ��ǥ�� ���� �ʿ� �������� ����.
		UE_LOG(LogTemp, Warning, TEXT("SnapToGrid : No Cell %d %d"), GridX, GridY);
		return WorldLocation;
	}

	// �ش� ��ǥ�� ���� �ʿ� �����Ѵٸ�
	//�� ��ǥ�� ��ġ�� �̿��ؼ� ���̰��� ���Ѵ�.
	Height = GridCells[Key].TerrainHeight;
	//UE_LOG(LogTemp, Warning, TEXT("Grid X : %d , Grid Y : %d"), GridX, GridY);

	// �ش� ��ǥ�� ���� �ѱ��.
	FVector result = FVector(GridOrigin.X + (GridX * GridSize), GridOrigin.Y + (GridY * GridSize), Height);

	//UE_LOG(LogTemp, Warning, TEXT("CellCenter : %s"), *result.ToString());
	return result;
}

bool UGridManager::TrySnapToGrid(FVector WorldLocation, FVector& OutSnapedLocation)
{
	// SnapToGrid �� �����丵 �Լ�
	// �ش� �Լ��� �����ϴ� ���� ���������� ��ȯ���� �Ű������� �ٸ���.

	// �����̼� ��ǥ�� ��� ��� ��ǥ�� ���Ѵ�.
	int32 GridX = FMath::RoundToInt((WorldLocation.X - GridOrigin.X) / GridSize);
	int32 GridY = FMath::RoundToInt((WorldLocation.Y - GridOrigin.Y) / GridSize);

	GridX = FMath::Clamp(GridX, 0, GridCount - 1);
	GridY = FMath::Clamp(GridY, 0, GridCount - 1);

	FIntPoint Key(GridX, GridY);
	if (GridCells.Contains(Key))
	{
		float Height = GridCells[Key].TerrainHeight;
		OutSnapedLocation = FVector(GridOrigin.X + (GridX * GridSize), 
									GridOrigin.Y + (GridY * GridSize),
									Height);
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SnapToGrid : No Cell %d %d"), GridX, GridY);
		return false;
	}
}


// ���� ���� ���� ���¸� ��ȯ�Ѵ�.
// ���� Overlap�� �����Ѵ�.
bool UGridManager::CanPlaceBuilding(FVector Location, int32 Width, int32 Height)
{
	// ���� ��ġ�� ���� �ε����� ��ȯ
	// ���� ���� ������ �˻�
	int32 GridX = FMath::RoundToInt((Location.X - GridOrigin.X) / GridSize);
	int32 GridY = FMath::RoundToInt((Location.Y - GridOrigin.Y) / GridSize);
	
	// ������ Width x Height ������ ���� ��ȯ�ϸ鼭 ���� ���θ� �˻�
	for (int32 i = 0; i < Width; i++)
	{
		for (int32 j = 0; j < Height; j++)
		{
			// üũ�� ��ǥ�� �����ؼ� ����.
			FIntPoint CheckCoords(GridX + i, GridY + j);
			FGridCell* Cell = GetCellAtGridCoords(CheckCoords);
			if (!Cell)
			{
				return false;
			}

			if (Cell->bIsOccupied)
			{
				return false;
			}
		}
	}

	// Overlap �˻� : �ش� �ǹ� ��ġ ������ ���� �˻�
	FVector BoxCenter = SnapToGrid(Location);
	FVector BoxExtent = FVector(Width * GridSize * 0.5f, Height * GridSize * 0.5f, 50.0f);

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(CanPlaceBuilding), false);
	//�ʿ��� ��� ��ġ ��� ���ͳ� �̹� ĳ�̵� ������Ʈ�� �����մϴ�.
	//QueryParams.AddIgnoredActor(Ghostmesh);

	//FCollisionObjectQueryParams ObjectQueryParams;
	//ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel1);

	bool bHasOverlap = GetWorld()->OverlapMultiByObjectType
	(
		OverlapResults,
		BoxCenter,
		FQuat::Identity,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldStatic),
		FCollisionShape::MakeBox(BoxExtent),
		QueryParams
	);

	//DrawDebugBox(GetWorld(), BoxCenter, BoxExtent, FColor::Green, false, 5.f, 0, 2.f);

	if (bHasOverlap)
	{
		for (const FOverlapResult& Result : OverlapResults)
		{
			AActor* OverlapActor = Result.GetActor();
			if (!OverlapActor || !OverlapActor->IsA(ABaseCraft::StaticClass()))
			{
				bHasOverlap = false;
			}
		}
	}
	return !bHasOverlap;
}

// ���� ���� �����Ǿ����� �ƴ����� �����Ѵ�.
// ���� ���
void UGridManager::MarkCellsOccupied(FVector Location, int32 Width, int32 Height, bool bOccupied)
{
	//Location�� �ǹ��� �߾����� �����Ǿ� �ִٰ� ������ ���,
	// ���� �� ��ǥ�� ���ϱ� ���� �ǹ� ũ���� ������ ���ش�.
	float OffsetX = (Width * GridSize) * 0.5f;
	float OffsetY = (Height * GridSize) * 0.5f;
	FVector StartLocation = Location - FVector(OffsetX, OffsetY, 0);

	int32 GridX = FMath::RoundToInt((StartLocation.X - GridOrigin.X) / GridSize);
	int32 GridY = FMath::RoundToInt((StartLocation.Y - GridOrigin.Y) / GridSize);

	for (int32 i = 0; i < Width; i++)
	{
		for (int32 j = 0; j < Height; j++)
		{
			// üũ�� ��ǥ�� �����ؼ� ����.
			FIntPoint Coords(GridX + i, GridY + j);
			FGridCell* Cell = GetCellAtGridCoords(Coords);
			if (Cell)
			{
				//�ش� ���� �����Ѵٸ� �Ű������� ���� ���·� ��ȯ�Ѵ�.
				Cell->bIsOccupied = bOccupied;
			}
		}
	}

}

// ��ǥ�� �̿��ؼ� ���� ���� �����´�.
FGridCell* UGridManager::GetCellAtGridCoords(FIntPoint GridCoords)
{
	// �ش� ��ǥ�� �̷���� ���� �����Ѵٸ�
	if (GridCells.Contains(GridCoords))
	{
		// �ش� ���� ��ȯ�Ѵ�.
		return &GridCells[GridCoords];
	}

	return nullptr;
}

// �ͷ����� ���� �����ͼ� �ݿ��ϱ�
float UGridManager::GetTerrainHeight(FVector WorldLocation)
{
	if (!GetWorld())
	{
		// ���� ���带 �������� ���Ѵٸ�.
		UE_LOG(LogTemp, Warning, TEXT("No World at GridManager"));
		return WorldLocation.Z;
	}

	// ���� Ʈ���̽� �غ�
	FHitResult HitResult;
	FVector Start = FVector(WorldLocation.X, WorldLocation.Y, 100000.f);
	FVector End = FVector(WorldLocation.X, WorldLocation.Y, -100000.f);

	FCollisionQueryParams TraceParams(FName(TEXT("HeightTrace")), true);
	TraceParams.bTraceComplex = true;	//������ �ݸ��� ���� Ȱ��ȭ.

	//Ư�� �±װ� �ִ� ���͵� ���� ����
	TArray<AActor*> IgnoreActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("IgnoreTrace"),
		IgnoreActors);

	for (AActor* Ignore : IgnoreActors)
	{
		TraceParams.AddIgnoredActor(Ignore);
	}

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, TraceParams))
	{
		// ���� Ʈ���̽� ���� ��, Hit �� ���� �ִٸ�
		AActor* HitActor = HitResult.GetActor();

		/*DrawDebugLine(GetWorld(), Start, End, FColor::Red, true, 2.0f, 0, 1.0f);
		DrawDebugPoint(GetWorld(), HitResult.Location, 10.0f, FColor::Blue, true, 2.0f);
		UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitResult.GetActor()->GetName());*/

		if (HitActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitActor->GetName());
		}
		
		// HitActor �� LandScape �϶��� ���� �ݿ�
		if (HitActor && HitActor->IsA(ALandscape::StaticClass()))
		{
			UE_LOG(LogTemp, Log, TEXT("Terrain hit at Z : %f"), HitResult.Location.Z);
			return HitResult.Location.Z;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[Error] No Hit at (%.1f, %.1f)"), WorldLocation.X, WorldLocation.Y);
	}


	return GridOrigin.Z;
}




// ����� �ڽ� �׸���
void UGridManager::DrawDebugGrid(UWorld* World)
{
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("����� �׷����� ����"));
		return;
	}

	for (int32 X = 0; X < GridCount; X++)
	{
		for (int32 Y = 0; Y < GridCount; Y++)
		{
			FGridCell* NewCell = GetCellAtGridCoords(FIntPoint(X, Y));

			if (NewCell == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("New Cell Null"));
				continue;
			}

			// ���� ������ǥ�� ���� ����
			float x = NewCell->WorldLocation.X;
			float y = NewCell->WorldLocation.Y;
			float z = NewCell->TerrainHeight;

			FVector BoxExtent = FVector(GridSize * 0.45f, GridSize * 0.45f, 5.0f);

			FVector BoxCenter = FVector(x, y, z);

			//DrawDebugBox(World, BoxCenter , BoxExtent, FColor::Green, true, 5.0f,0,2.f);
		}
	}
}

// ���� �׸��� ��Į ����
void UGridManager::CreateGridDecals(UWorld* InWorld, UMaterialInterface* DecalMaterial)
{
	if (!InWorld || !DecalMaterial)
	{
		UE_LOG(LogTemp, Warning, TEXT("Decal: No World, No DecalMat"));
		return;
	}

	if (!InWorld->IsNetMode(NM_DedicatedServer))
	{
		// �̹� �����ϴ� decal Component �� �ִٸ� ����
		for (UDecalComponent* ExistingDecal : GridDecals)
		{
			if (ExistingDecal)
			{
				ExistingDecal->DestroyComponent();
			}
		}
		GridDecals.Empty();

		// �� ���� ���ο� ��Į ����
		// ���鿡 �����ϰ� �򸮴� ��Į�� �����ϱ� ���� ȸ���� -90���� ����
		FRotator DecalRotation(-90.0f, 0.f, 0.f);

		//��Į ũ��
		FVector DecalSize(200.0f, GridSize * 0.5f, GridSize * 0.5f);

		for (auto& Elem : GridCells)
		{
			const FGridCell& Cell = Elem.Value;
			FVector SpawnLocaiton = Cell.WorldLocation;

			// ���鿡 ���̷���, �� ��ġ���� �ణ ���� �÷��� ����� ����
			SpawnLocaiton.Z += 2.f;

			UDecalComponent* SpawnedDecal = UGameplayStatics::SpawnDecalAtLocation(
				InWorld,
				DecalMaterial,
				DecalSize,
				SpawnLocaiton,
				DecalRotation,
				0.f				//LifeSpan : ���������� ��Į�� ����.
			);

			if (SpawnedDecal)
			{
				// ���ٿ��� �� ���̵��� FadeScreenSize�� �۰� ���� ( �⺻�� 0.01 )
				SpawnedDecal->SetFadeScreenSize(0.001f);

				// �ʿ�� SortOrder ���� �߰� ����


				// ��Į ���� ����
				GridDecals.Add(SpawnedDecal);
			}
		}

	}
}



// �ͷ����� �������� ���� ��� �ʱ�ȭ�ϱ�.
void UGridManager::InitializeGridFromTerrain(AActor* TerrainActor, int32 DesiredCellSize)
{

	if (GridCells.Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("GridManager already initialized!"));
		return;
	}

	if (!TerrainActor || DesiredCellSize <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("InitializeError"));
		return;
	}

	//LandScape�� ��ü �ٿ�� �ڽ��� ������
	FBox Bounds = TerrainActor->GetComponentsBoundingBox();
	FVector Min = Bounds.Min;
	FVector Max = Bounds.Max;
	FVector TerrainSize = Max - Min;

	GridOrigin = FVector(Min.X, Min.Y, Min.Z);

	// �� ���� ũ�� ( ���ϴ� ��)
	GridSize = DesiredCellSize;

	// ���� ���� (����, ����)
	// ��) ���� ������ �� �ø� ó��
	GridCount = FMath::CeilToInt(TerrainSize.X / GridSize);

	GridCells.Empty();

	// ���� ���� : x �� y �������� GridCount�� ����
	for (int32 X = 0; X < GridCount; X++)
	{
		for (int32 Y = 0; Y < GridCount; Y++)
		{
			FIntPoint GridCoords(X, Y);
			FVector CellBase = GridOrigin + FVector(X * GridSize, Y * GridSize, 0);

			// �� ���� ���̴� LineTrace �� Terrain �����͸� ���� ������ �� ����.
			float Height = GetTerrainHeight(CellBase);
			FVector CellLocation = CellBase;
			CellLocation.Z = Height;

			FGridCell NewCell;
			NewCell.WorldLocation = CellLocation;
			NewCell.TerrainHeight = Height;
			NewCell.bIsOccupied = false;

			GridCells.Add(GridCoords, NewCell);
			UE_LOG(LogTemp, Warning, TEXT("MapCount : %d"), static_cast<int>(GridCells.Num()));
		}
	}


}
