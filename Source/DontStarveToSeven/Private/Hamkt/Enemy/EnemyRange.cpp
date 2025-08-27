// Fill out your copyright notice in the Description page of Project Settings.


#include "Hamkt/Enemy/EnemyRange.h"
#include "Engine/World.h"						// 월드 액세스, 스폰용
#include "GameFramework/Actor.h"				// 액터 스폰 파라미터
#include "Components/SkeletalMeshComponent.h"	// 컴포넌트 Attach
#include "AIController.h"
#include "Hamkt/Enemy/AI/AIC_EnemyBase.h"
#include "GameFramework/CharacterMovementComponent.h"	// GetCharacterMovement() 반환타입을 사용하기 위해 필수
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AEnemyRange::AEnemyRange()
{
}


void AEnemyRange::BeginPlay()
{
	Super::BeginPlay();
}

// 캐릭터 상황별 이동속도
void AEnemyRange::SetMovementSpeed_Implementation(E_MovementSpeed NewSpeed)
{
	float SpeedValue = 0.f;	// 이동속도 값

	// 열거형에 따라 이동속도 값을 설정
	switch (NewSpeed)
	{
	case E_MovementSpeed::Idle:
		SpeedValue = 0.f;
		break;
	case E_MovementSpeed::Walking:
		SpeedValue = 200.f;
		break;
	case E_MovementSpeed::Jogging:
		SpeedValue = 300.f;
		break;
	case E_MovementSpeed::Sprinting:
		SpeedValue = 500.f;
		break;
	default:
		break;
	}

	// CharacterMovementComponent를 통해 MakWalkSpeed 변경
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	if (!IsValid(MoveComp)) return;		// 유효값 검증
	MoveComp->MaxWalkSpeed = SpeedValue;
	
}

// 최소&최대 이동&공격거리
void AEnemyRange::GetIdealRange_Implementation(float& AttackRadius, float& DefendRadius) const
{
	AttackRadius = 600.f;
	DefendRadius = 600.f;
}

///////////////////////////////////////////////////////////////////////////
// 
// 공격 인터페이스 재정의함수
void AEnemyRange::Attack_Implementation(FName NotifyName)
{
	ServerAttackMontage(NotifyName);
}

void AEnemyRange::ServerAttackMontage_Implementation(const FName& NotifyName)
{
	MulticastAttackMontage(NotifyName);
}


void AEnemyRange::MulticastAttackMontage_Implementation(const FName& NotifyName)
{
	// GetSkeletalMeshComponent() 또는 Mesh 등으로 AnimInstance를 가져옴
	USkeletalMeshComponent* SkeletalMeshComp = GetMesh();
	if (!IsValid(AttackMontage)) return;		// EquipMontage가 유효하지 않으면 리턴

	// 캐릭터의 스켈레탈 메시에서 애님 인스턴스를 가져옴
	UAnimInstance* AnimInstance = SkeletalMeshComp->GetAnimInstance();
	if (!IsValid(AnimInstance)) return;		// 애님인스턴스가 유효하지 않으면 리턴

	float PlayRate = 1.2f;	// 몽타주 재생속도


	// 장착 몽타주를 재생,  재생 성공시 Duration은 0보다 큰 값 반환
	float Duration = AnimInstance->Montage_Play(AttackMontage, PlayRate);
	if (Duration > 0.f)
	{
		// Montage 종료 델리게이트 설정
		FOnMontageEnded MontageEndedDelegate;
		MontageEndedDelegate.BindUObject(this, &AEnemyRange::OnMontageEnded);
		AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, AttackMontage);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AttackMontage Montage not played properly."));
	}

	//////
	// 노티파이 이름이 Attack 이라면
	if (NotifyName == "Attack")
	{
		if (!IsValid(WeaponClass)) return;	// WeaponClass가 할당되지 않았다면 실패처리

		AAIController* AIController = Cast<AAIController>(GetController());	// Self - GetAIController 와 같음
		if (!IsValid(AIController)) return;

		AAIC_EnemyBase* EnemyAIController = Cast<AAIC_EnemyBase>(AIController);	// AI Controller를 형변환
		if (!IsValid(EnemyAIController)) return;

		AActor* Target = EnemyAIController->GetAttackTargetActor();	// 어택 타겟 키값을 가져옴
		if (!IsValid(Target)) return;

		UWorld* World = GetWorld();
		if (!IsValid(World))
		{
			UE_LOG(LogTemp, Error, TEXT("GetWorld() returned nullptr in MulticastAttackMontage."));
			return;
		}

		if (!IsValid(WeaponClass))
		{
			UE_LOG(LogTemp, Error, TEXT("WeaponClass is not valid!"));
			return;
		}

		UE_LOG(LogTemp, Warning, TEXT("Spawning weapon using WeaponClass: %s"), *WeaponClass->GetName());
		UE_LOG(LogTemp, Warning, TEXT("Spawn location: %s"), *GetActorTransform().GetLocation().ToString());

		// 무기 스폰 & 소켓에 어태치
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// 던질 투사체의 트랜스폼을 해당 액터의 트랜스폼을 기준으로 세팅
		FTransform SpawnTransform = GetActorTransform();

		AActor* SpawnedWeapon = World->SpawnActor<AActor>(WeaponClass, SpawnTransform, SpawnParams);
		if (!IsValid(SpawnedWeapon))
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to spawn WeaponActor in MulticastAttackMontage."));
			return;
		}
		WeaponActor = SpawnedWeapon;
		UE_LOG(LogTemp, Warning, TEXT("Weapon spawned successfully: %s"), *WeaponActor->GetName());

		FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);
		SpawnedWeapon->AttachToComponent(
			GetMesh(),
			AttachRules,
			FName("hand_r_throw_socket")
		);

		if (WeaponActor && Target)
		{
			// UINTERFACE 기반의 인터페이스 함수는 IBPI_EnemyAI::Execute_GetIdealRange()를 사용해야 함
			IBPI_EnemyAI::Execute_ThrowWeaponAtTarget(this, WeaponActor, Target);
		}

		
	}
}


///////////////////////////////////////////////////////////////////////////
// 

// 포물선 (발사체 액터, 타겟액터)
void AEnemyRange::ThrowWeaponAtTarget_Implementation(AActor* InWeapon, AActor* InTarget)
{
	ServerThrowWeaponAtTarget(InWeapon, InTarget);
}

void AEnemyRange::ServerThrowWeaponAtTarget_Implementation(AActor* InWeapon, AActor* InTarget)
{
	if (!InWeapon || !InTarget)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid weapon or target."));
		return;
	}

	// 1) 소켓에서 분리 (Detach)
	InWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	// 2) 무기에 ProjectileMovementComponent 찾기
	UProjectileMovementComponent* ProjComp = InWeapon->FindComponentByClass<UProjectileMovementComponent>();
	if (!ProjComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("WeaponActor has no ProjectileMovementComponent!"));
		return;
	}

	// 3) 시작점 / 목표점 계산
	FVector StartLoc = InWeapon->GetActorLocation();
	FVector EndLoc = InTarget->GetActorLocation();

	// 4) SuggestProjectileVelocity 파라미터 설정
	float LaunchSpeed = 1000.f;           // 초기 속도 (900 cm/s)
	bool bFavorHighArc = true;           // 높은 포물선 솔루션을 우선 선택
	float OverrideGravityZ = 300.f;        // 기본 중력보다 낮은 값(예: 700 cm/s²)을 적용하여 아크를 높임
	float CollisionRadius = 0.f;         // 충돌 반지름은 사용하지 않음

	FVector TossVelocity;
	bool bHasSolution = UGameplayStatics::SuggestProjectileVelocity(
		this,
		TossVelocity,
		StartLoc,
		EndLoc,
		LaunchSpeed,
		bFavorHighArc,
		OverrideGravityZ,
		CollisionRadius,
		ESuggestProjVelocityTraceOption::DoNotTrace,
		FCollisionResponseParams::DefaultResponseParam,
		TArray<AActor*>(),
		false,
		true
	);

	if (bHasSolution)
	{
		// 5) 계산된 속도를 ProjectileMovementComponent에 설정
		ProjComp->Velocity = TossVelocity;
		UE_LOG(LogTemp, Log, TEXT("Weapon thrown with velocity: %s"), *TossVelocity.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No valid projectile solution found! Throwing weapon forward instead."));
		FVector ForwardThrow = (EndLoc - StartLoc).GetSafeNormal() * ProjComp->InitialSpeed;
		ProjComp->Velocity = ForwardThrow;
	}
}

void AEnemyRange::MulticastThrowWeaponAtTarget_Implementation(AActor* InWeapon, AActor* InTarget)
{
}