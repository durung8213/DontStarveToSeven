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
	// 배열에 저장된 데칼 컴포넌트가 있다면 모두 제거한다.
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
	//월드 좌표를 격자 인덱스로 변환
	// 해당 로케이션 좌표의 평균 가운데 좌표를 구한다.
	int32 GridX = FMath::RoundToInt((WorldLocation.X - GridOrigin.X) / GridSize);
	int32 GridY = FMath::RoundToInt((WorldLocation.Y - GridOrigin.Y) / GridSize);

	GridX = FMath::Clamp(GridX, 0, GridCount - 1);
	GridY = FMath::Clamp(GridY, 0, GridCount - 1);

	// 격자 내 해당 셀의 높이 정보 사용
	FIntPoint Key(GridX, GridY);

	float Height = GridOrigin.Z;

	if (!GridCells.Contains(Key))
	{
		// 해당 좌표가 현재 맵에 존재하지 않음.
		UE_LOG(LogTemp, Warning, TEXT("SnapToGrid : No Cell %d %d"), GridX, GridY);
		return WorldLocation;
	}

	// 해당 좌표가 현재 맵에 존재한다면
	//그 좌표의 위치를 이용해서 높이값을 구한다.
	Height = GridCells[Key].TerrainHeight;
	//UE_LOG(LogTemp, Warning, TEXT("Grid X : %d , Grid Y : %d"), GridX, GridY);

	// 해당 좌표의 값을 넘긴다.
	FVector result = FVector(GridOrigin.X + (GridX * GridSize), GridOrigin.Y + (GridY * GridSize), Height);

	//UE_LOG(LogTemp, Warning, TEXT("CellCenter : %s"), *result.ToString());
	return result;
}

bool UGridManager::TrySnapToGrid(FVector WorldLocation, FVector& OutSnapedLocation)
{
	// SnapToGrid 의 리팩토링 함수
	// 해당 함수와 실행하는 것이 동일하지만 반환값과 매개변수가 다르다.

	// 로케이션 좌표의 평균 가운데 좌표를 구한다.
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


// 현재 셀의 점유 상태를 반환한다.
// 동적 Overlap도 실행한다.
bool UGridManager::CanPlaceBuilding(FVector Location, int32 Width, int32 Height)
{
	// 시작 위치를 격자 인덱스로 반환
	// 사전 격자 데이터 검사
	int32 GridX = FMath::RoundToInt((Location.X - GridOrigin.X) / GridSize);
	int32 GridY = FMath::RoundToInt((Location.Y - GridOrigin.Y) / GridSize);
	
	// 지정된 Width x Height 영역의 셀을 순환하면서 점유 여부를 검사
	for (int32 i = 0; i < Width; i++)
	{
		for (int32 j = 0; j < Height; j++)
		{
			// 체크할 좌표를 생성해서 설정.
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

	// Overlap 검사 : 해당 건물 배치 영역에 대해 검사
	FVector BoxCenter = SnapToGrid(Location);
	FVector BoxExtent = FVector(Width * GridSize * 0.5f, Height * GridSize * 0.5f, 50.0f);

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(CanPlaceBuilding), false);
	//필요한 경우 배치 대상 액터나 이미 캐싱된 오브젝트를 무시합니다.
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

// 셀이 현재 점유되었는지 아닌지를 설정한다.
// 수정 요망
void UGridManager::MarkCellsOccupied(FVector Location, int32 Width, int32 Height, bool bOccupied)
{
	//Location은 건물의 중앙으로 보정되어 있다고 가정할 경우,
	// 시작 셀 좌표를 구하기 위해 건물 크기의 절반을 빼준다.
	float OffsetX = (Width * GridSize) * 0.5f;
	float OffsetY = (Height * GridSize) * 0.5f;
	FVector StartLocation = Location - FVector(OffsetX, OffsetY, 0);

	int32 GridX = FMath::RoundToInt((StartLocation.X - GridOrigin.X) / GridSize);
	int32 GridY = FMath::RoundToInt((StartLocation.Y - GridOrigin.Y) / GridSize);

	for (int32 i = 0; i < Width; i++)
	{
		for (int32 j = 0; j < Height; j++)
		{
			// 체크할 좌표를 생성해서 설정.
			FIntPoint Coords(GridX + i, GridY + j);
			FGridCell* Cell = GetCellAtGridCoords(Coords);
			if (Cell)
			{
				//해당 셀이 존재한다면 매개변수로 받은 상태로 전환한다.
				Cell->bIsOccupied = bOccupied;
			}
		}
	}

}

// 좌표를 이용해서 셀의 값을 가져온다.
FGridCell* UGridManager::GetCellAtGridCoords(FIntPoint GridCoords)
{
	// 해당 좌표로 이루어진 셀이 존재한다면
	if (GridCells.Contains(GridCoords))
	{
		// 해당 셀을 반환한다.
		return &GridCells[GridCoords];
	}

	return nullptr;
}

// 터레인의 높이 가져와서 반영하기
float UGridManager::GetTerrainHeight(FVector WorldLocation)
{
	if (!GetWorld())
	{
		// 만약 월드를 가져오지 못한다면.
		UE_LOG(LogTemp, Warning, TEXT("No World at GridManager"));
		return WorldLocation.Z;
	}

	// 라인 트레이스 준비
	FHitResult HitResult;
	FVector Start = FVector(WorldLocation.X, WorldLocation.Y, 100000.f);
	FVector End = FVector(WorldLocation.X, WorldLocation.Y, -100000.f);

	FCollisionQueryParams TraceParams(FName(TEXT("HeightTrace")), true);
	TraceParams.bTraceComplex = true;	//복잡한 콜리전 감지 활성화.

	//특정 태그가 있는 액터들 전부 무시
	TArray<AActor*> IgnoreActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("IgnoreTrace"),
		IgnoreActors);

	for (AActor* Ignore : IgnoreActors)
	{
		TraceParams.AddIgnoredActor(Ignore);
	}

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, TraceParams))
	{
		// 라인 트레이스 실행 뒤, Hit 된 것이 있다면
		AActor* HitActor = HitResult.GetActor();

		/*DrawDebugLine(GetWorld(), Start, End, FColor::Red, true, 2.0f, 0, 1.0f);
		DrawDebugPoint(GetWorld(), HitResult.Location, 10.0f, FColor::Blue, true, 2.0f);
		UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitResult.GetActor()->GetName());*/

		if (HitActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitActor->GetName());
		}
		
		// HitActor 가 LandScape 일때만 높이 반영
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




// 디버그 박스 그리기
void UGridManager::DrawDebugGrid(UWorld* World)
{
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("디버그 그려지지 않음"));
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

			// 셀의 월드좌표와 지형 높이
			float x = NewCell->WorldLocation.X;
			float y = NewCell->WorldLocation.Y;
			float z = NewCell->TerrainHeight;

			FVector BoxExtent = FVector(GridSize * 0.45f, GridSize * 0.45f, 5.0f);

			FVector BoxCenter = FVector(x, y, z);

			//DrawDebugBox(World, BoxCenter , BoxExtent, FColor::Green, true, 5.0f,0,2.f);
		}
	}
}

// 격자 그리드 데칼 생성
void UGridManager::CreateGridDecals(UWorld* InWorld, UMaterialInterface* DecalMaterial)
{
	if (!InWorld || !DecalMaterial)
	{
		UE_LOG(LogTemp, Warning, TEXT("Decal: No World, No DecalMat"));
		return;
	}

	if (!InWorld->IsNetMode(NM_DedicatedServer))
	{
		// 이미 존재하는 decal Component 가 있다면 정리
		for (UDecalComponent* ExistingDecal : GridDecals)
		{
			if (ExistingDecal)
			{
				ExistingDecal->DestroyComponent();
			}
		}
		GridDecals.Empty();

		// 각 셀에 새로운 데칼 생성
		// 지면에 평평하게 깔리는 데칼을 생성하기 위해 회전을 -90도로 설정
		FRotator DecalRotation(-90.0f, 0.f, 0.f);

		//데칼 크기
		FVector DecalSize(200.0f, GridSize * 0.5f, GridSize * 0.5f);

		for (auto& Elem : GridCells)
		{
			const FGridCell& Cell = Elem.Value;
			FVector SpawnLocaiton = Cell.WorldLocation;

			// 지면에 붙이려면, 셀 위치보다 약간 위로 올려야 깔끔히 보임
			SpawnLocaiton.Z += 2.f;

			UDecalComponent* SpawnedDecal = UGameplayStatics::SpawnDecalAtLocation(
				InWorld,
				DecalMaterial,
				DecalSize,
				SpawnLocaiton,
				DecalRotation,
				0.f				//LifeSpan : 영구적으로 데칼을 유지.
			);

			if (SpawnedDecal)
			{
				// 원근에서 잘 보이도록 FadeScreenSize를 작게 설정 ( 기본값 0.01 )
				SpawnedDecal->SetFadeScreenSize(0.001f);

				// 필요시 SortOrder 등을 추가 설정


				// 데칼 참조 보관
				GridDecals.Add(SpawnedDecal);
			}
		}

	}
}



// 터레인을 기준으로 격자 모양 초기화하기.
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

	//LandScape의 전체 바운딩 박스를 가져옴
	FBox Bounds = TerrainActor->GetComponentsBoundingBox();
	FVector Min = Bounds.Min;
	FVector Max = Bounds.Max;
	FVector TerrainSize = Max - Min;

	GridOrigin = FVector(Min.X, Min.Y, Min.Z);

	// 각 셀의 크기 ( 원하는 값)
	GridSize = DesiredCellSize;

	// 격자 개수 (가로, 세로)
	// 예) 정수 나눗셈 후 올림 처리
	GridCount = FMath::CeilToInt(TerrainSize.X / GridSize);

	GridCells.Empty();

	// 격자 생성 : x 와 y 방향으로 GridCount씩 생성
	for (int32 X = 0; X < GridCount; X++)
	{
		for (int32 Y = 0; Y < GridCount; Y++)
		{
			FIntPoint GridCoords(X, Y);
			FVector CellBase = GridOrigin + FVector(X * GridSize, Y * GridSize, 0);

			// 각 셀의 높이는 LineTrace 나 Terrain 데이터를 통해 가져올 수 있음.
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
