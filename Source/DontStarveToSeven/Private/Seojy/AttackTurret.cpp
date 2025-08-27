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


	//��Ʈ ������Ʈ ���� 
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = RootComp;

	//���� �޽� ������Ʈ�� ����
	TurretBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretBase"));
	TurretBase->SetupAttachment(RootComponent);
	TurretSwivel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretSwivel"));
	TurretSwivel->SetupAttachment(TurretBase);
	TurretHead = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretHead"));
	TurretHead->SetupAttachment(TurretSwivel);

	//����ƽ �޽� ���� ã�Ƽ� �����ϱ�.(Base)
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

	//���Ǿ� ������Ʈ ���� �� ����
	//���Ǿ� ������Ʈ�� ���� ��, �ش� �ּҰ��� ���� ������ ������ ���۷��� ����
	DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
	DetectionSphere->InitSphereRadius(1000.f); //���� �ݰ� ����

	// **���� ��� => �����ϰ� �� ������ �ݸ��� �������� �̸��� ����
	DetectionSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	DetectionSphere->SetupAttachment(RootComponent);	//�ش� ���Ǿ ��Ʈ ������Ʈ�� ����.

	

	// ���� ���׸��� ����
	ActualMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/Game/Seojy/Fab/Turret/arma.arma'"));


	// ź�� Ŭ���� ����
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

	// �ʱ� ���� ����
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

	


	//�� ������ ���� ������ �̺�Ʈ�� ���ε� �صд�.
	if (HasAuthority() && DetectionSphere && CurrentMode == ECraftMode::Actual)
	{
		//Overlap �̺�Ʈ
		DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &AAttackTurret::OnEnemyDetected);
		DetectionSphere->OnComponentEndOverlap.AddDynamic(this, &AAttackTurret::OnEnemyLost);

		// �ֱ� �˻�
		GetWorldTimerManager().SetTimer(TimerHandle_TargetCheck, this,
			&AAttackTurret::PeriodicTargetCheck, 0.5f, true);

		// �ʱ� ����
		TArray<AActor*> InitialActors;
		DetectionSphere->GetOverlappingActors(InitialActors, AActor::StaticClass());

		// �ʱ� overlap üũ : �ͷ��� ������ �� �̹� ���� ���� �ִ� ������ Targets �迭�� �߰�
		for (AActor* Actor : InitialActors)
		{
			if (Actor && Actor != this && Actor->ActorHasTag(FName("Enemy"))
				&& !Actor->IsA(ABullet::StaticClass()) && !Targets.Contains(Actor))
			{
				Targets.AddUnique(Actor);
			}
		}

		UpdateTarget();

		//���� �ʱ� Ÿ���� �ִٸ� �߻� Ÿ�̸� ����
		if (Targets.Num() > 0)
		{
			GetWorldTimerManager().SetTimer(TimerHandle_Fire, this, &AAttackTurret::TryFire, FireRate, true);
		}
	}

	// ���� ������Ʈ�� �ִ� ������ ���� ���� �������� �ɽ�Ʈ
	UDTS_BaseInteractionUserWidget* InteractionUserWidget = Cast<UDTS_BaseInteractionUserWidget>(InteractionWidget->GetWidget());
	if (InteractionUserWidget) {
		// ������ �ɽ�Ʈ �Ǹ�
		InteractionUserWidget->SetInteractionText(FText::FromString(TEXT("Fire")));	// ������ ���� �ý�Ʈ ����
		InteractionUserWidget->SetCollapsedWidget(); // ������ �Ⱥ��̰� ��
	}

	UE_LOG(LogTemp, Log, TEXT("AttackTurret BeginPlay - Role : %d, RemoteRole : %d"),
		int32(GetLocalRole()), int32(GetRemoteRole()));

}

void AAttackTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ���������� ȸ�� ���
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

		// ���� ó�� ���� �����Ǹ� �߻� Ÿ�̸� ����
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

	// ��尡 Actual�� ��ȯ�Ǹ� ������ �̺�Ʈ�� Ÿ�̸Ӹ� �缳��
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
		// Ghost���� ��ȯ�� Ÿ�̸� �� �̺�Ʈ ���� �� �ʿ��� ó���� ����
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

// Ÿ���� ������Ʈ
void AAttackTurret::UpdateTarget()
{	

	// ���� ��Ʈ ����϶��� �ڵ� ȸ���� �̷������ �ʵ��� ����.
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
		//���� Ÿ�̸Ӱ� �������� �ƴ϶��
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

// ���� ���� ��, ���� ���ؼ� ȸ��
void AAttackTurret::UpdateRotation(float DeltaTime)
{
	// �������� / ���߾� ��� �϶� ����ǵ���.
	if (!HasAuthority() || CurrentMode == ECraftMode::Ghost)
		return;

	FRotator DesiredRotation = DefaultRotation;

	if (CurrentTarget && bIsInAttackMode)
	{
		FVector Pivot = TurretSwivel->GetComponentLocation();
		FVector DirToTarget = (CurrentTarget->GetActorLocation() - Pivot).GetSafeNormal();
		
		// �ͷ��� �⺻ ȸ������ ����
		DesiredRotation = DirToTarget.Rotation() + FRotator(0.f, -90.f, 0.f);

	}

	////���� ȸ������ ��ǥ ȸ������ �����Ͽ� �ε巴�� ȸ��
	FRotator CurrentSwivelRot = TurretSwivel->GetComponentRotation();
	FRotator NewSwivelRot = FMath::RInterpTo(
		CurrentSwivelRot,
		DesiredRotation,
		DeltaTime,
		5.0f
	);

	// ���������� ȸ�� + ���� ���� ����
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
		// �������� ��ź ��� ����
		ServerFire();
	}
	else
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_Fire);
		//UE_LOG(LogTemp, Log, TEXT("Try Fire Failed"));
	}
}

// ��ź �߻�
void AAttackTurret::ServerFire_Implementation()
{	// Multicast RPC�� ����� ��ź �߻� ����Ʈ ( �Ҹ�, ��ƼŬ) �� ��� Ŭ���̾�Ʈ�� ����

	// ���� ���ݸ�尡 �ƴ϶�� �������� �ʴ´�.
	if (!bIsInAttackMode || !CurrentTarget)
	{
		bIsCanFire = false;
		// ���� ��尡 �ƴѵ�, Ÿ�̸Ӱ� �������̶�� �����Ѵ�.
		if (GetWorldTimerManager().IsTimerActive(TimerHandle_Fire))
		{
			GetWorldTimerManager().ClearTimer(TimerHandle_Fire);
		}
		return;
	}
	// �ͷ� ����� �ֽ� ��ȯ ������Ʈ
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
	//������ ���Ǿ ���ٸ� ����.
	if (!DetectionSphere || !HasAuthority())
		return;

	// 1. ���� ������ ������ �ִ� ���͵��� �����´�. 
	TArray<AActor*> OverlappingActors;
	DetectionSphere->GetOverlappingActors(OverlappingActors, AActor::StaticClass());

	// �⺻ Ÿ�� �迭�� ����
	//Targets.Empty();

	// 2. ���� Target �迭�� ��ȸ�ϸ鼭, ���� overlappingActors�� ���Ե��� �ʰų� ��ȿ���� ���� ���͸� .
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

	// Ÿ���� ã�� �迭�� ä���. 
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
	
	// 4. Ÿ�� �迭�� �������  CurrentTarget �� ���� ��带 ������Ʈ
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
	// ���� ��ź ������ �������� ����
	// �ͷ� ����� ��ġ�� ȸ������ �������� ��ź ����
	FVector SpawnLocation;
	FRotator SpawnRotation;

	if (TurretSwivel)
	{
		TurretHead->UpdateComponentToWorld();
	}

	// ������ ��ġ�� ����� ��ź ����
	SpawnLocation = TurretHead->GetSocketLocation("Muzzle");
	//�ͷ� ȸ�� ������ ����
	SpawnRotation = TurretHead->GetSocketRotation("Muzzle");

	// ��ź ���� �Ķ���� 
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;



	// ��ź ���� ���� ( BulletClass �� ��ȿ�ؾ� ��)
	ABullet* Projectile = GetWorld()->SpawnActor<ABullet>(BulletClass, SpawnLocation, SpawnRotation, SpawnParams);

	if (Projectile && CurrentTarget)
	{
		Projectile->ProjectileMovement->bIsHomingProjectile = true;
		// Ÿ���� rootComponent �� ����
		Projectile->ProjectileMovement->HomingTargetComponent = CurrentTarget->GetRootComponent();

		// Homing Acceleration Magnitude �� �Ѿ��� Ÿ���� ������� ���� ���ӵ�
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
	// Ŭ���̾�Ʈ���� TurretHeadRot�� ���ŵ� �븶�� �� �Լ��� ȣ���
	if (TurretHead)
	{ 
		TurretSwivel->SetWorldRotation(TurretHeadRot);
	}
}
