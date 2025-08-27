// Fill out your copyright notice in the Description page of Project Settings.


#include "Seojy/AttackTurret.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Kimmh/HUDnUI/UI/DTS_BaseInteractionUserWidget.h"


AAttackTurret::AAttackTurret()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;


	//루트 컴포넌트 설정 
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = RootComp;

	//정적 메시 컴포넌트를 생성
	TurretBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretBase"));
	TurretBase->SetupAttachment(RootComponent);
	TurretSwivel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretSwivel"));
	TurretSwivel->SetupAttachment(TurretBase);
	TurretHead = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretHead"));
	TurretHead->SetupAttachment(TurretSwivel);

	//스태틱 메시 에셋 찾아서 설정하기.(Base)
	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshBase
	(TEXT("StaticMesh'/Game/Seojy/Fab/Turret/MG_turret_portable_Stand.MG_turret_portable_Stand'"));
	if (MeshBase.Succeeded())
	{
		TurretBase->SetStaticMesh(MeshBase.Object);
		ActorMeshComponents.Add(TurretBase);
	}

	//Head
	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshHead
	(TEXT("StaticMesh'/Game/Seojy/Fab/Turret/MG_turret_portable_MGMain.MG_turret_portable_MGMain'"));
	if (MeshHead.Succeeded())
	{
		TurretHead->SetStaticMesh(MeshHead.Object);
		ActorMeshComponents.Add(TurretHead);
	}

	// Swivel
	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshSwivel
	(TEXT("StaticMesh'/Game/Seojy/Fab/Turret/MG_turret_portable_Swivel.MG_turret_portable_Swivel'"));
	if (MeshSwivel.Succeeded())
	{
		TurretSwivel->SetStaticMesh(MeshSwivel.Object);
		ActorMeshComponents.Add(TurretSwivel);
	}

	FVector CurrentLocation = TurretBase->GetRelativeLocation();
	CurrentLocation.Z += 53.0f;
	TurretBase->SetRelativeLocation(CurrentLocation);

	//스피어 컴포넌트 생성 및 설정
	//스피어 컴포넌트를 생성 후, 해당 주소값을 왼쪽 포인터 변수에 레퍼런스 저장
	DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
	DetectionSphere->InitSphereRadius(1000.f); //감지 반경 설정

	// **수정 요망 => 감지하게 될 액터의 콜리전 프로파일 이름을 설정
	DetectionSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	DetectionSphere->SetupAttachment(RootComponent);	//해당 스피어를 루트 컴포넌트에 붙임.

	

	// 원본 머테리얼 저장
	ActualMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/Game/Seojy/Fab/Turret/arma.arma'"));


	// 탄알 클래스 지정
	static ConstructorHelpers::FClassFinder<ABullet> BulletBPClass(
		TEXT("Blueprint'/Game/Seojy/BluePrint/Object/BP_Bullet.BP_Bullet_C'"));
	if (BulletBPClass.Succeeded())
	{
		BulletClass = BulletBPClass.Class;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Fail To Cast Bullet Class"));
	}

	// 초기 상태 설정
	bIsInAttackMode = false;
	CurrentTarget = nullptr;
	FireRate = 2.0f;
}

AAttackTurret::~AAttackTurret()
{
}


void AAttackTurret::BeginPlay()
{
	Super::BeginPlay();

	DefaultRotation = this->GetActorRotation();

	CraftName = "BP_AttackTurret";

	SetBuildingData(1, 1, ECraftType::AttackTurret);

	


	//적 감지를 위한 오버랩 이벤트를 바인딩 해둔다.
	if (HasAuthority() && DetectionSphere && CurrentMode == ECraftMode::Actual)
	{
		//Overlap 이벤트
		DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &AAttackTurret::OnEnemyDetected);
		DetectionSphere->OnComponentEndOverlap.AddDynamic(this, &AAttackTurret::OnEnemyLost);

		// 주기 검사
		GetWorldTimerManager().SetTimer(TimerHandle_TargetCheck, this,
			&AAttackTurret::PeriodicTargetCheck, 0.5f, true);

		// 초기 상태
		TArray<AActor*> InitialActors;
		DetectionSphere->GetOverlappingActors(InitialActors, AActor::StaticClass());

		// 초기 overlap 체크 : 터렛이 생성될 떄 이미 영역 내에 있는 적들을 Targets 배열에 추가
		for (AActor* Actor : InitialActors)
		{
			if (Actor && Actor != this && Actor->ActorHasTag(FName("Enemy"))
				&& !Actor->IsA(ABullet::StaticClass()) && !Targets.Contains(Actor))
			{
				Targets.AddUnique(Actor);
			}
		}

		UpdateTarget();

		//만약 초기 타겟이 있다면 발사 타이머 시작
		if (Targets.Num() > 0)
		{
			GetWorldTimerManager().SetTimer(TimerHandle_Fire, this, &AAttackTurret::TryFire, FireRate, true);
		}
	}

	// 위젯 컴포넌트에 있는 위젯이 내가 만든 위젯으로 케스트
	UDTS_BaseInteractionUserWidget* InteractionUserWidget = Cast<UDTS_BaseInteractionUserWidget>(InteractionWidget->GetWidget());
	if (InteractionUserWidget) {
		// 위젯이 케스트 되면
		InteractionUserWidget->SetInteractionText(FText::FromString(TEXT("Fire")));	// 위젯에 넣을 택스트 설정
		InteractionUserWidget->SetCollapsedWidget(); // 위젯이 안보이게 함
	}

	UE_LOG(LogTemp, Log, TEXT("AttackTurret BeginPlay - Role : %d, RemoteRole : %d"),
		int32(GetLocalRole()), int32(GetRemoteRole()));

}

void AAttackTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 서버에서만 회전 계산
	if (HasAuthority() && CurrentMode == ECraftMode::Actual
		&& bIsInAttackMode && CurrentTarget)
	{
		UpdateRotation(DeltaTime);
	}

	if (HasAuthority() && bIsCanFire)
	{
		//SpawnProjectile();
	}

}


void AAttackTurret::OnEnemyDetected(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority()) return;

	if (OtherActor && OtherActor != this && OtherActor->ActorHasTag(FName("Enemy"))
		&& !OtherActor->IsA(ABullet::StaticClass())
		&& !Targets.Contains(OtherActor))
	{
		Targets.AddUnique(OtherActor);
		UpdateTarget();

		// 만약 처음 적이 감지되면 발사 타이머 시작
		if (Targets.Num() == 1)
		{
			GetWorldTimerManager().SetTimer(TimerHandle_Fire, this, &AAttackTurret::TryFire,
				FireRate, true);
		}
	}
}

void AAttackTurret::OnEnemyLost(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!HasAuthority())
		return;

	if (OtherActor && OtherActor != this && OtherActor->ActorHasTag(FName("Enemy")))
	{
		bool bRemoved = Targets.Remove(OtherActor) > 0;

		Targets.Remove(OtherActor);
		UE_LOG(LogTemp, Log, TEXT("OnEnemyLost : Removed : %s removed: %d"), 
			*OtherActor->GetName(),bRemoved);

		UpdateTarget();


		if (Targets.Num() == 0)
		{
			GetWorldTimerManager().ClearTimer(TimerHandle_Fire);
		}
	}
}

void AAttackTurret::SetCraftMode(ECraftMode NewMode)
{
	ABaseCraft::SetCraftMode(NewMode);

	// 모드가 Actual로 전환되면 오버랩 이벤트와 타이머를 재설정
	if (NewMode == ECraftMode::Actual)
	{
		if (DetectionSphere)
		{
			DetectionSphere->OnComponentBeginOverlap.Clear();
			DetectionSphere->OnComponentEndOverlap.Clear();

			DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &AAttackTurret::OnEnemyDetected);
			DetectionSphere->OnComponentEndOverlap.AddDynamic(this, &AAttackTurret::OnEnemyLost);
		}

		if (HasAuthority())
		{
			GetWorldTimerManager().SetTimer(TimerHandle_TargetCheck, this, &AAttackTurret::PeriodicTargetCheck, 0.5f, true);
		}
	}
	else
	{
		// Ghost모드로 전환시 타이머 및 이벤트 해제 등 필요한 처리를 수행
		if (HasAuthority())
		{
			GetWorldTimerManager().ClearTimer(TimerHandle_TargetCheck);
			GetWorldTimerManager().ClearTimer(TimerHandle_Fire);
		}
	}
}

void AAttackTurret::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAttackTurret, bIsInAttackMode);
	DOREPLIFETIME(AAttackTurret, CurrentTarget);
	DOREPLIFETIME(AAttackTurret, TurretHeadRot);
}

// 타겟을 업데이트
void AAttackTurret::UpdateTarget()
{	

	// 현재 고스트 모드일때는 자동 회전이 이루어지지 않도록 고정.
	if (CurrentMode == ECraftMode::Ghost)
		return;

	if (Targets.Num() == 0)
	{
		CurrentTarget = nullptr;
		bIsInAttackMode = false;
		bIsCanFire = false;
	}
	else if(Targets.Num() > 0)
	{
		CurrentTarget = Targets[0];
		bIsInAttackMode = (CurrentTarget != nullptr);
		bIsCanFire = bIsInAttackMode;
	}

	if (bIsInAttackMode)
	{
		//아직 타이머가 동작중이 아니라면
		if (!GetWorldTimerManager().IsTimerActive(TimerHandle_Fire))
		{
			GetWorldTimerManager().SetTimer(
				TimerHandle_Fire,
				this,
				&AAttackTurret::TryFire,
				FireRate,
				true
			);
		}
	}
	else
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_Fire);
	}


	OnRep_TurretState();
}

// 적이 있을 때, 적을 향해서 회전
void AAttackTurret::UpdateRotation(float DeltaTime)
{
	// 서버전용 / 액추얼 모드 일때 실행되도록.
	if (!HasAuthority() || CurrentMode == ECraftMode::Ghost)
		return;

	FRotator DesiredRotation = DefaultRotation;

	if (CurrentTarget && bIsInAttackMode)
	{
		FVector Pivot = TurretSwivel->GetComponentLocation();
		FVector DirToTarget = (CurrentTarget->GetActorLocation() - Pivot).GetSafeNormal();
		
		// 터렛의 기본 회전값을 보정
		DesiredRotation = DirToTarget.Rotation() + FRotator(0.f, -90.f, 0.f);

	}

	////현재 회전에서 목표 회전으로 보간하여 부드럽게 회전
	FRotator CurrentSwivelRot = TurretSwivel->GetComponentRotation();
	FRotator NewSwivelRot = FMath::RInterpTo(
		CurrentSwivelRot,
		DesiredRotation,
		DeltaTime,
		5.0f
	);

	// 서버에서만 회전 + 복제 변수 갱신
	TurretSwivel->SetWorldRotation(NewSwivelRot);
	TurretHeadRot = NewSwivelRot;
}

void AAttackTurret::TryFire()
{
	UE_LOG(LogTemp, Log, TEXT("bIsInAttackMode : %d , CurrentTarget : %s , BulletClass : %s"),
		bIsInAttackMode,
		CurrentTarget ? *CurrentTarget->GetName() : TEXT("None"),
		BulletClass ? *BulletClass->GetName() : TEXT("None"));
	if (bIsInAttackMode && CurrentTarget && BulletClass && bIsCanFire)
	{
		//UE_LOG(LogTemp, Log, TEXT("Try Fire Success"));
		// 서버에서 포탄 쏘기 실행
		ServerFire();
	}
	else
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_Fire);
		//UE_LOG(LogTemp, Log, TEXT("Try Fire Failed"));
	}
}

// 포탄 발사
void AAttackTurret::ServerFire_Implementation()
{	// Multicast RPC를 사용해 포탄 발사 이펙트 ( 소리, 파티클) 를 모든 클라이언트에 전달

	// 현재 공격모드가 아니라면 공격하지 않는다.
	if (!bIsInAttackMode || !CurrentTarget)
	{
		bIsCanFire = false;
		// 공격 모드가 아닌데, 타이머가 실행중이라면 중지한다.
		if (GetWorldTimerManager().IsTimerActive(TimerHandle_Fire))
		{
			GetWorldTimerManager().ClearTimer(TimerHandle_Fire);
		}
		return;
	}
	// 터렛 헤드의 최신 변환 업데이트
	if (TurretSwivel)
	{
		TurretSwivel->UpdateComponentToWorld();
	}

	SpawnProjectile();
	
}


void AAttackTurret::OnRep_TurretState()
{
	//UE_LOG(LogTemp, Log, TEXT("UpdateTarget : bIsInAttackMode : %d"), bIsInAttackMode);
}

void AAttackTurret::PeriodicTargetCheck()
{
	//오버랩 스피어가 없다면 리턴.
	if (!DetectionSphere || !HasAuthority())
		return;

	// 1. 현재 오버랩 범위에 있는 액터들을 가져온다. 
	TArray<AActor*> OverlappingActors;
	DetectionSphere->GetOverlappingActors(OverlappingActors, AActor::StaticClass());

	// 기본 타겟 배열을 배우고
	//Targets.Empty();

	// 2. 기존 Target 배열을 순회하면서, 현재 overlappingActors에 포함되지 않거나 유효하지 않은 액터를 .
	for (int32 i = Targets.Num() - 1; i >= 0; --i)
	{
		AActor* OldTarget = Targets[i];
		if (!OverlappingActors.Contains(OldTarget) || !IsValid(OldTarget))
		{
			UE_LOG(LogTemp, Warning, TEXT("PeriodicTargetCheck: Removing invalid Target : %s"),
				*Targets[i]->GetName());
			Targets.RemoveAt(i);
		}
	}

	FVector SphereCenter = DetectionSphere->GetComponentLocation();
	float SphereRadiusSq = FMath::Square(DetectionSphere->GetScaledSphereRadius());

	Targets.Empty();

	// 타겟을 찾아 배열을 채운다. 
	for (AActor* Actor : OverlappingActors)
	{
		if (Actor && Actor != this && Actor->ActorHasTag(FName("Enemy"))
			&& !Actor->IsA(ABullet::StaticClass()) 
			&& !Targets.Contains(Actor))
		{
			float DistSq = FVector::DistSquared(Actor->GetActorLocation(), SphereCenter);
			if (DistSq <= SphereRadiusSq)
			{
				Targets.AddUnique(Actor);
			}
			UE_LOG(LogTemp, Warning, TEXT("PeriodicTargetCheck : Adding new Target : %s"), *Actor->GetName());
		}
	}
	
	// 4. 타겟 배열을 기반으로  CurrentTarget 및 공격 모드를 업데이트
	UpdateTarget();


	if (Targets.Num() == 0)
	{
		bIsInAttackMode = false;
		bIsCanFire = false;
		GetWorldTimerManager().ClearTimer(TimerHandle_Fire);
		//UE_LOG(LogTemp, Log, TEXT("PeriodicTargetCheck NO : No Target"));
	}
	else 
	{
		bIsInAttackMode = true;
		if (!GetWorldTimerManager().IsTimerActive(TimerHandle_Fire))
		{
			bIsCanFire = true;
			GetWorldTimerManager().SetTimer(TimerHandle_Fire, this, &AAttackTurret::TryFire, FireRate, true);
			//UE_LOG(LogTemp, Log, TEXT("PeriodicTargetCheck : Target Present"));
		}
	}
}

void AAttackTurret::SpawnProjectile()
{
	// 실제 포탄 생성은 서버에서 수행
	// 터렛 헤드의 위치와 회전값을 기준으로 포탄 스폰
	FVector SpawnLocation;
	FRotator SpawnRotation;

	if (TurretSwivel)
	{
		TurretHead->UpdateComponentToWorld();
	}

	// 소켓의 위치를 사용해 포탄 스폰
	SpawnLocation = TurretHead->GetSocketLocation("Muzzle");
	//터렛 회전 보정값 적용
	SpawnRotation = TurretHead->GetSocketRotation("Muzzle");

	// 포탄 생성 파라미터 
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;



	// 포탄 액터 스폰 ( BulletClass 가 유효해야 함)
	ABullet* Projectile = GetWorld()->SpawnActor<ABullet>(BulletClass, SpawnLocation, SpawnRotation, SpawnParams);

	if (Projectile && CurrentTarget)
	{
		Projectile->ProjectileMovement->bIsHomingProjectile = true;
		// 타겟의 rootComponent 를 설정
		Projectile->ProjectileMovement->HomingTargetComponent = CurrentTarget->GetRootComponent();

		// Homing Acceleration Magnitude 는 총알이 타겟을 따라잡기 위한 가속도
		Projectile->ProjectileMovement->HomingAccelerationMagnitude = 5000.f;
		//UE_LOG(LogTemp, Log, TEXT("Bullet Create"));
	}
	else
	{
		//UE_LOG(LogTemp, Log, TEXT("NO Bullet Create"));
	}

	
}

void AAttackTurret::OnRep_TurretHeadRot()
{
	// 클라이언트에서 TurretHeadRot이 갱신될 대마다 이 함수가 호출됨
	if (TurretHead)
	{ 
		TurretSwivel->SetWorldRotation(TurretHeadRot);
	}
}
