// Fill out your copyright notice in the Description page of Project Settings.


#include "Seojy/CraftManager.h"
#include "Seojy/AttackTurret.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/CollisionProfile.h"
#include "Kimmh/Character/Controller/DTS_GamePlayPlayerController.h"
#include "Kimmh/BasePlayerCharacter.h"
#include "Leesh/SHGameModeBase.h"

// Sets default values for this component's properties
UCraftManager::UCraftManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;

	// GridManager 생성 
	GridManager = ObjectInitializer.CreateDefaultSubobject<UGridManager>(this, TEXT("GridManager"));


	/*static ConstructorHelpers::FObjectFinder<UMaterialInterface> DecalMat(
		TEXT("/Game/Seojy/Material/M_Decal.M_Decal"));
	if (DecalMat.Succeeded())
	{
		GridDecalMaterialSoft = DecalMat.Object;
	}*/
}


// Called when the game starts
void UCraftManager::BeginPlay()
{
	Super::BeginPlay();

	// PlayerController 설정
	OwnerPlayer = Cast<ABasePlayerCharacter>(GetOwner());
	if (OwnerPlayer)
	{
		PlayerPC = Cast<ADTS_GamePlayPlayerController>(OwnerPlayer->GetController());

		if (PlayerPC)
		{
			UE_LOG(LogTemp, Log, TEXT("PC Cast Success"));
		}
	}

	// NavMesh 가져오기
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANavMeshBoundsVolume::StaticClass(),
		NavVolume);

}


// Called every frame
void UCraftManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCraftManager::SetGhostMesh(ECraftType CraftType)
{
	GhostMesh = GetWorld()->SpawnActor<ABaseCraft>
		(
			GetActorClass(CraftType),
			FVector::ZeroVector,
			FRotator::ZeroRotator
		);

	if (GhostMesh)
	{
		GhostMesh->SetActorEnableCollision(false);
		UE_LOG(LogTemp, Warning, TEXT("No GhostMesh"));
	}
}

// Craft 버튼을 누르면 실행되는 함수
void UCraftManager::StartCraftMode(ECraftType craftType)
{
	// 해당 레벨에서 LandScape를 가져오기.
	TArray<AActor*> FoundTerrains;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALandscape::StaticClass(), FoundTerrains);
	AActor* TerrainActor = nullptr;

	if (FoundTerrains.Num() > 0)
	{
		TerrainActor = FoundTerrains[0];
	}


	// 격자 시스템 초기화 
	if (GridManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("GridManager 유효"));

		//명시적으로 월드 포인터를 GridManager에 전달
		GridManager->SetWorld(GetWorld());

		//터레인을 기준으로 격자 초기화
		if (TerrainActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("TerrainActor 유효"));
			GridManager->GridCells.Empty();

			// 셀 크기 200 으로 초기화
			GridManager->InitializeGridFromTerrain(TerrainActor, 200);
			UE_LOG(LogTemp, Warning, TEXT("그리드 매니저 생성"));

			// 디버그용 그리드 출력
			//GridManager->DrawDebugGrid(GetWorld());

			// 클라이언트 용 그리드 데칼 생성
			Client_SpawnGridDecal();
		}
		else 
		{
			UE_LOG(LogTemp, Warning, TEXT("TerrainActor 없음"));
		}

	}

	// GhostMesh 생성
	if (!GhostMesh)
	{
		// 해당 Enum 타입에 맞는 클래스 가져오기
		UClass* craftClass = GetActorClass(craftType);
		if (!craftClass)
		{
			UE_LOG(LogTemp, Warning, TEXT("NO Class!!"));
			return;
		}

		// 고스트 메쉬 생성
		GhostMesh = GetWorld()->SpawnActor<ABaseCraft>
			(
				GetActorClass(craftType),
				FVector::ZeroVector,
				FRotator::ZeroRotator
			);

		if (GhostMesh)
		{
			// 고스트 메쉬 콜리전 해제
			GhostMesh->SetActorEnableCollision(false);
		}


		// 타이머 실행
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_UpdateGhostMesh, this,
			&UCraftManager::UpdateGhostMesh, 0.1f, true);
	}


}

void UCraftManager::EndCraftMode()
{

	ClientEndCraftMode();
}

void UCraftManager::Client_SpawnGridDecal_Implementation()
{
	SpawnGridDecal();
}

void UCraftManager::ClientEndCraftMode_Implementation()
{
	// 클라이언트에서 타이머와 고스트 메시 제거
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_UpdateGhostMesh);

	// 고스트 메쉬 제거 또는 다른 처리
	if (GhostMesh)
	{
		GhostMesh->Destroy();
		GhostMesh = nullptr;
	}

	// 데칼 삭제/Script/Engine.Material'/Game/Seojy/Material/M_Decal.M_Decal'
	if (GridManager)
	{
		GridManager->RemoveGridDecals();
	}


	ADTS_GamePlayPlayerController* CurrentPlayerController = Cast<ADTS_GamePlayPlayerController>(Cast<ABasePlayerCharacter>(GetOwner())->GetController());
	if (CurrentPlayerController) {
		UE_LOG(LogTemp, Warning, TEXT("Success Get Controller"));
		CurrentPlayerController->ChangeInputMappingContext(EIMC::DefaultIMC);
	}

	UE_LOG(LogTemp, Warning, TEXT("EndCraft Mode"));
}



// Enum 타입으로 지정된 변수에 따라 액터의 클래스를 반환한다. 해당 클래스로 액터 스폰
TSubclassOf<AActor> UCraftManager::GetActorClass(ECraftType craftType)
{
	TSubclassOf<AActor> classType;

	switch (craftType)
	{
	case ECraftType::AttackTurret: 
		classType = CraftTypeArr[0];
		break;
	case ECraftType::BonFire:
		classType = CraftTypeArr[1];
		break;
	case ECraftType::WoodFence:
		classType = CraftTypeArr[2];
		break;
	case ECraftType::HealTurret:
		classType = CraftTypeArr[3];
		break; 
	default:
		classType = nullptr;
		break;
	}

	return classType;
}

// 건물 설치를 시도한다.
// 서버에서 실행하도록.

void UCraftManager::TryPlaceBuilding()
{
	if (!GetOwner() || GetOwner()->HasAuthority() == false)
	{
		// 서버 권한이 아니라면 서버 RPC 호출
		ServerTryPlaceBuilding(
			GhostMesh->BuildingData.BuildingType,
			GhostMesh->bIsCanBuild,
			GhostMesh->BuildingData.Width,
			GhostMesh->BuildingData.Height,
			GhostMesh->GetActorRotation(),
			GhostMesh->GetActorLocation()
		);
		return;
	}

	// 이미 서버라면 직접 실행
	ServerTryPlaceBuilding_Implementation(
		GhostMesh->BuildingData.BuildingType,
		GhostMesh->bIsCanBuild,
		GhostMesh->BuildingData.Width,
		GhostMesh->BuildingData.Height,
		GhostMesh->GetActorRotation(),
		GhostMesh->GetActorLocation()
	);

}

// 실제로 서버에서 해당 건물을 설치
// GhostMode -> Actual Mode로
void UCraftManager::ServerTryPlaceBuilding_Implementation(
	ECraftType CraftType, bool bCanBuild, int32 Width, int32 Height, FRotator Rotation, FVector PlacementLocation)
{
	if (!GridManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlaceFail"));
		return;
	}

	if (GhostMesh)
	{
		if (CraftType == ECraftType::None)
		{
			UE_LOG(LogTemp, Warning, TEXT("PlaceFail"));
			return;
		}
	}

	if (GridManager->CanPlaceBuilding(PlacementLocation, Width, Height) && bCanBuild)
	{
		UE_LOG(LogTemp, Warning, TEXT("Current bIsCanBuild : %d"), bCanBuild);
		// 현재 고스트 메쉬의 회전 반영
		FRotator DesiredRotation = Rotation;

		//실제 아이템 생성
		ABaseCraft* SpawnedBuilding = GetWorld()->SpawnActor<ABaseCraft>
			(
				GetActorClass(CraftType),
				PlacementLocation,
				DesiredRotation
			);
		if (SpawnedBuilding)
		{
			//해당 점유 상태를 true로 변환한다.
			GridManager->MarkCellsOccupied(PlacementLocation, Width, Height, true);

			//해당 머테리얼과 모드를 바꾼다.
			SpawnedBuilding->SetCraftMode(ECraftMode::Actual);
			SpawnedBuilding->InteractionWidget->SetRelativeLocation(FVector(0.f, 0.0f, 0.f));

			// 생성된 건축물 정보 서버에 저장
			ASHGameModeBase* GM = Cast<ASHGameModeBase>(GetWorld()->GetAuthGameMode());
			if(!GM)
			{
				UE_LOG(LogTemp, Warning, TEXT("No GameMode"));
				EndCraftMode();
				return;
			}

			GM->UpdateBuildingInfo(SpawnedBuilding->CraftID, SpawnedBuilding->CraftName, SpawnedBuilding->GetCraftHP(), SpawnedBuilding->GetActorTransform());

			UE_LOG(LogTemp, Warning, TEXT("SuccessPlaceBuilding"));
			EndCraftMode();
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("해당 위치에 건물을 배치할 수 없습니다."));
	}
}

bool UCraftManager::ServerTryPlaceBuilding_Validate(
	ECraftType CraftType, bool bCanBuild, int32 Width, int32 Height, FRotator Rotation, FVector PlacementLocation)
{
	return true;
}

// 계속해서 고스트 메쉬를 업데이트 한다.
void UCraftManager::UpdateGhostMesh()
{
	if (!GhostMesh || !GridManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("GhostMesh, GhostManager null"));
		return;
	}

	if (GridManager->GetGridCells().Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("GridManager has no CELL"));
	}


	// 그리드 매니저에서 셀 형식으로 로케이션을 가져온다.
	// 플레이어 시점(LineTrace)를 통한 배치 위치 결정 
	FVector PlacementLocation = GetPlacementLocation();

	//고스트 메쉬 설치
	GhostMesh->SetActorLocation(PlacementLocation);

	FVector BoxCenter = PlacementLocation;
	FVector BoxExtent = FVector(GridManager->GridSize * 0.5f, GridManager->GridSize * 0.5f, 2.0f);

	TArray<FOverlapResult> OverlapResult;
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;
	//QueryParams.AddIgnoredActor(OwnerPlayer);
	// 자기 자신을 제외
	QueryParams.AddIgnoredActor(GhostMesh);

	// Nav 메쉬 무시
	for (AActor* Volume : NavVolume)
	{
		QueryParams.AddIgnoredActor(Volume);
	}


	// 필요시 배치 대상 액터를 무시하도록
	bool bHasOverlap = GetWorld()->OverlapMultiByObjectType(
		OverlapResult,
		BoxCenter,
		FQuat::Identity,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldStatic),
		FCollisionShape::MakeBox(BoxExtent),
		QueryParams
	);

	for (auto& OverItem : OverlapResult)
	{

		AActor* HitActor = OverItem.GetActor();

		// 랜드 스케이프 무시
		if (HitActor->IsA(ALandscape::StaticClass()))
		{
			bHasOverlap = false;
			continue;
		}

		ECollisionChannel HitChannel = HitActor->GetRootComponent()->GetCollisionObjectType();
		if (HitChannel == ECC_WorldStatic)
		{
			bHasOverlap = true;
			break;
		}

		// 크래프트 아이템일 경우 충돌 처리
		if (HitActor->IsA(ABaseCraft::StaticClass()) || HitActor->ActorHasTag("Craft"))
		{
			bHasOverlap = true;
			break;
		}
	}

	// 플레이어와의 거리를 측정
	float DistToPlayer = FVector::Dist(PlacementLocation, OwnerPlayer->GetActorLocation());
	bool bTooCloseToPlayer = (DistToPlayer < 80.f);

	//최종 겹침 여부
	bool bFinalOverlap = bHasOverlap || bTooCloseToPlayer;

	//DrawDebugBox(GetWorld(), BoxCenter, BoxExtent, FColor::Red, false, 5.f, 0, 2.f);

	//만약 현재 전달받은 위치가 건물을 세울 수 있는 위치라면
	//Overlap 검사 결과에 따라 GHostMesh 머티리얼 변경
	GhostMesh->SetPreviewColor(!bFinalOverlap);

	// 건물 설치 가능 여부 설정
	GhostMesh->bIsCanBuild = !bFinalOverlap;
}

// 프리뷰 건물의 위치를 카메라에서 나오는 라인 트레이스를 활용하여 쏜다.
// 무조건 격자 내 중심점에 위치하도록 스냅한다.
FVector UCraftManager::GetPlacementLocation()
{
	if (!PlayerPC)
		return GetOwner()->GetActorLocation();

	FVector CameraLocation;
	FRotator CameraRotation;
	PlayerPC->GetPlayerViewPoint(CameraLocation, CameraRotation);

	FVector TraceStart = CameraLocation;
	FVector TraceEnd = TraceStart + (CameraRotation.Vector() * 8000.f);

	FHitResult Hit;
	FCollisionQueryParams TraceParams;
	//TraceParams.AddIgnoredActor(OwnerPlayer);
	TraceParams.AddIgnoredActor(GhostMesh);

	// 카메라 (플레이어 시선) 라인 트레이스 실행
	if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, TraceParams))
	{
		//DrawDebugLine(GetWorld(), TraceStart, Hit.Location, FColor::Blue, false, 0.1f, 0, 2.0f);

		AActor* HitActor = Hit.GetActor();
		if (HitActor && HitActor->IsA(ALandscape::StaticClass()))
		{
			FVector SnappedLocation;
			
			// 현재 충돌된 위치를 셀 격자의 중심점 위치로 스냅
			if (GridManager->TrySnapToGrid(Hit.Location, SnappedLocation))
			{
				return SnappedLocation;
			}
			else
			{
				//격자 밖이라면
				UE_LOG(LogTemp, Warning, TEXT("GetPlacementLocation : Snap 실패"));
			}

		}

	}
	return GetOwner()->GetActorLocation();
}

// 건물 우측  회전
void UCraftManager::RotateRightGhostMesh()
{
	if (GhostMesh)
	{
		FRotator CurrentRotation = GhostMesh->GetActorRotation();
		CurrentRotation.Yaw += 90.0f;
		GhostMesh->SetActorRotation(CurrentRotation);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GhostMeshNull"));
	}
}

// 건물 좌측 회전
void UCraftManager::RotateLeftGhostMesh()
{
	if (GhostMesh)
	{
		FRotator CurrentRotation = GhostMesh->GetActorRotation();
		CurrentRotation.Yaw -= 90.0f;
		GhostMesh->SetActorRotation(CurrentRotation);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GhostMeshNull"));
	}
}

void UCraftManager::SpawnGridDecal()
{

	//데칼용 머테리얼 로드
	UMaterialInterface* GridDecalMat = nullptr;

	if (GridDecalMaterialSoft.IsValid())
	{
		GridDecalMat = GridDecalMaterialSoft.Get();
		UE_LOG(LogTemp, Warning, TEXT("캐시된 머테리얼 사용"));
	}

	if (!GridDecalMat)
	{
		GridDecalMat = GridDecalMaterialSoft.LoadSynchronous();

		UE_LOG(LogTemp, Warning, TEXT("데칼 머테리얼 : LoadSynchronous"));
	}

	if (!GridDecalMat)
	{
		GridDecalMat = LoadObject<UMaterialInterface>
			(
				nullptr, TEXT("/Game/Seojy/Material/M_Decal.M_Decal")
			);

		UE_LOG(LogTemp, Warning, TEXT("데칼 머테리얼 : LoadObject"));

	}


	// 클라이언트 전용으로 실행
	if (GridDecalMat)
	{
		UE_LOG(LogTemp, Warning, TEXT("데칼 존재"));
		if (GridManager)
		{
			GridManager->CreateGridDecals(GetWorld(), GridDecalMat);
			UE_LOG(LogTemp, Warning, TEXT("데칼 생성 완료"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("GridManager 유효하지 않음"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("데칼 머테리얼 유효하지 않음"));
	}

	UE_LOG(LogTemp, Warning, TEXT("Client_SpawnGridDecal 실행: NetMode %d"), (int32)GetWorld()->GetNetMode());
}

