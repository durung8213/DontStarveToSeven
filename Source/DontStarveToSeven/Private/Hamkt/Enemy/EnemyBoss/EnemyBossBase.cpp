// Fill out your copyright notice in the Description page of Project Settings.


#include "Hamkt/Enemy/EnemyBoss/EnemyBossBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Hamkt/Enemy/AI/AIC_EnemyBossBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Hamkt/Enemy/AI/BPI_EnemyAI.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Hamkt/Enemy/AI/Enums/E_AIState.h"


// Sets default values
AEnemyBossBase::AEnemyBossBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add(TEXT("Enemy"));

	AutoPossessAI = EAutoPossessAI::PlacedInWorld;

	// Pawn이 사용할 AIController 클래스를 명시적으로 지정합니다.
	AIControllerClass = AAIC_EnemyBossBase::StaticClass();

	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// 캡슐 컴포넌트 크기 조정
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	// 42.f: 캡슐 반경, 96.f: 캡슐 반높이 (필요에 따라 값 조정)

	// 스켈레톤 매쉬의 상대 위치를 캡슐에 맞게 조정
	// 보통 캐릭터의 메쉬는 캡슐의 중앙보다 위에 있으므로, 아래로 오프셋을 주어 캡슐과 자연스럽게 맞춥니다.
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -96.f));

	// Z축 Yaw 기준 회전
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	// 매쉬 스케일 조정 (필요 시 변경)
	GetMesh()->SetWorldScale3D(FVector(3.f, 3.f, 3.f));

	// 플레이어와 카메라 충돌 방지. 카메라 Collision Ignore 
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = true;

}

void AEnemyBossBase::ServerDie_Implementation()
{
	MulticastDie();
}

// 보스가 착지시 모래 소환
	// 불규칙한 바닥 대비용
void AEnemyBossBase::SpawnAndDestroyNiagaraEffect_Implementation()
{
	if (!GetWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnAndDestroyNiagaraEffect: GetWorld() is null."));
		return;
	}

	if (!NiagaraActorClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnAndDestroyNiagaraEffect: NiagaraActorClass is not set!"));
		return;
	}

	// 원하는 스폰 위치 (예: 보스의 현재 위치)
	FVector SpawnLocation = GetActorLocation();

	// 라인 트레이스를 이용해서 보스 아래 500 거리까지 탐색
	FHitResult HitResult;
	FVector TraceStart = SpawnLocation;
	FVector TraceEnd = SpawnLocation - FVector(0.f, 0.f, 500.f);

	// 바닥의 콜리전 채널을 사용
	if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility))
	{
		SpawnLocation = HitResult.Location;
	}
	else
	{
		// 해당 방법이 실패할 경우 캡슐 높이를 빼는 고정 오프셋 방법으로 전환
		SpawnLocation.Z -= GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	}

	FRotator SpawnRotation = GetActorRotation();

	// 스폰 파라미터 설정
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Niagara 액터 스폰
	AActor* SpawnedNiagara = GetWorld()->SpawnActor<AActor>(NiagaraActorClass, SpawnLocation, SpawnRotation, SpawnParams);
	if (!SpawnedNiagara)
	{
		UE_LOG(LogTemp, Error, TEXT("SpawnAndDestroyNiagaraEffect: Failed to spawn NiagaraActor."));
		return;
	}

	// (옵션) 보스 캐릭터에 Attach: 원하는 경우 KeepWorldTransform 또는 SnapToTarget 적용
	SpawnedNiagara->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

	// 1초 뒤에 SpawnedNiagara를 파괴하는 타이머 설정

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [SpawnedNiagara]()
		{
			if (IsValid(SpawnedNiagara))
			{
				SpawnedNiagara->Destroy();
			}
		}, 0.7f, false);
}

// JumpAttack 행동 후 발동
void AEnemyBossBase::Landed(const FHitResult& Hit)
{
	// 부모 클래스(기본 착지 처리) 먼저 호출
	Super::Landed(Hit);

	// 여기서 MovementMode를 Walking으로 전환
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->SetMovementMode(MOVE_Walking);
	}

	FString Message = FString::Printf(TEXT("Land good"));

	// 필요하다면, 점프 공격 종료 처리를 하거나
	// 위치를 미세 조정 (예: 오브젝트 관통 문제 있을 경우)

	ServerDamageNearByActorsOnLanding();

	// BP_SandPack 소환 및 1초 후 파괴
	SpawnAndDestroyNiagaraEffect();
}

void AEnemyBossBase::ServerSetBlackboardKey_Implementation()
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (!IsValid(AIController)) return;

	if (AIController->RunBehaviorTree(GetBehaviorTree()))
	{
		if (!IsValid(BlackboardComp))
		{
			BlackboardComp = AIController->GetBlackboardComponent();
			if (!IsValid(BlackboardComp))
			{
				UE_LOG(LogTemp, Warning, TEXT("BlackboardComponent is invalid!"));
				return;
			}
		}

		if (BossResetLocationActor)
			BlackboardComp->SetValueAsObject(TEXT("BossResetLocation"), BossResetLocationActor);

		if (BossResetFocusActor)
			BlackboardComp->SetValueAsObject(TEXT("BossResetFocus"), BossResetFocusActor);

	}
}

void AEnemyBossBase::ServerDamageNearByActorsOnLanding_Implementation()
{
	// 1. 착지 지점을 기준으로, 구체 범위 내의 액터를 검사 - RootComponent 기준
	FVector Center = GetActorLocation();
	float SphereRadius = 250.f * GetMesh()->GetComponentScale().X;	// 착지 지점 기준으로 부터의 스페어 범위

	TArray<FHitResult> HitResults;	// Hit한 대상을 TArray 배열로 담아옴
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_GameTraceChannel1);	// 플레이어

	FCollisionShape Sphere = FCollisionShape::MakeSphere(SphereRadius);

	// 트레이스 파라미터
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);	// 자기 자신은 무시

	bool bHit = GetWorld()->SweepMultiByObjectType(
		HitResults,
		Center,
		Center,
		FQuat::Identity,
		ObjectQueryParams,
		Sphere,
		QueryParams
	);

	// 히트된 대상이 있다면.
	if (bHit)
	{
		for (auto& HitResult : HitResults)
		{
			AActor* HitActor = HitResult.GetActor();
			if (IsValid(HitActor))	// 값이 유효하다면
			{
				// 액터가 해당 인터페이스를 구현하는가?
				if (HitActor->ActorHasTag(TEXT("Player"))
					&& HitActor->GetClass()->ImplementsInterface(UBPI_EnemyAI::StaticClass()))
				{

					FString Message = FString::Printf(TEXT("Boss 25F ATTACK !!!"));
					GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, Message);
					IBPI_EnemyAI::Execute_TakeDamage(HitActor, 25.f, this);	// HitActor 된 캐릭의 TakeDamage 함수를 가져와서 25 데미지를 입힌다.

				}
			}
		}
	}

	// 디버그 구체 시각화
	//DrawDebugSphere(GetWorld(), Center, SphereRadius, 12, FColor::Red, false, 2.0f);
}

// Called when the game starts or when spawned
void AEnemyBossBase::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyBossBase::ServerPlayMontage_Implementation(UAnimMontage* Montage, const FName& NotifyName)
{
	MultiPlayMontage(Montage, NotifyName);
}

void AEnemyBossBase::MultiPlayMontage_Implementation(UAnimMontage* Montage, const FName& NotifyName)
{
	// GetSkeletalMeshComponent() 또는 Mesh 등으로 AnimInstance를 가져옴
	USkeletalMeshComponent* SkeletalMeshComp = GetMesh();

	// AttackMontage가 할당 되어있는가?
	if (Montage)
	{
		// 캐릭터의 스켈레탈 메시에서 애님 인스턴스를 가져옴
		UAnimInstance* AnimInstance = SkeletalMeshComp->GetAnimInstance();
		float PlayRate = 1.2f;	// 몽타주 재생속도
		if (AnimInstance && Montage)	// 애님인스턴스가 유효하면
		{
			// 공격 몽타주를 재생,   재생 성공시 Duration은 0보다 큰 값 반환
			float Duration = AnimInstance->Montage_Play(Montage, PlayRate);
			if (Duration > 0.f)
			{
				// MontageEndedDelegate를 통해 애니메이션(몽타주) 재생이 언제 끝났는지 콜백으로 알 수 있음
				FOnMontageEnded MontageEndedDelegate;
				MontageEndedDelegate.BindUObject(this, &AEnemyBossBase::OnMontageEnded);
				AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, Montage);	// AnimBP쪽
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("AttackMontage is not set on %s"), *GetName());
		}
	}

	if (HasAuthority())
	{
		// 노티파이 이름이 Attack라면
		if (NotifyName == "Attack")
		{
			// **	 SweepSingleByObjectType  ==  SphereTraceForObjects		**//


			// 1. 시작과 끝 위치 & 구체 크기
			FVector Start = GetActorLocation();
			FVector End = Start + GetActorForwardVector() * 200.f;
			float SphereRadius = 25.f;	// 구체 크기

			// 2. 결과를 저장할 변수
			FHitResult HitResult;

			// 3. Pawn 타입 객체 대상으로 검색
			FCollisionObjectQueryParams ObjectQueryParams;
			ObjectQueryParams.AddObjectTypesToQuery(ECC_GameTraceChannel1);

			// 4. 트레이스파라미터 (자기자신 무시).. Self Ignore와 같은 기능
			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(this);

			// 5. Sphere Trace 실행
			bool bHit = GetWorld()->SweepSingleByObjectType(
				HitResult,		// 결과를 저장할 변수
				Start,			// 시작점
				End,			// 끝점
				FQuat::Identity,	// 회전값 (회전 0)
				ObjectQueryParams,	// 감시할 객체 타입들
				FCollisionShape::MakeSphere(SphereRadius),	// 구형 콜리전 쉐입
				QueryParams			// SelfIgnore와 같은 기능
			);

			// 6. 결과처리
				// Trace에 히트시
			if (bHit)
			{
				// 충돌한 액터
				AActor* HitActor = HitResult.GetActor();

				if (!IsValid(HitActor)) return;


				// 액터가 해당 인터페이스를 구현하는가?
				if (HitActor->ActorHasTag(TEXT("Player"))
					&& HitActor->GetClass()->ImplementsInterface(UBPI_EnemyAI::StaticClass()))
				{
					FString Message = FString::Printf(TEXT("Boss 25F ATTACK !!!"));
					GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, Message);
					IBPI_EnemyAI::Execute_TakeDamage(HitActor, 25.f, this);	// HitActor 된 캐릭의 TakeDamage 함수를 가져와서 25 데미지를 입힌다.
				}
			}

			// 시작점과 끝점의 구체를 그립니다.
			//DrawDebugSphere(GetWorld(), Start, SphereRadius, 12, FColor::Green, false, 2.0f, 0, 2.0f);
			//DrawDebugSphere(GetWorld(), End, SphereRadius, 12, FColor::Green, false, 2.0f, 0, 2.0f);

			// 두 점을 잇는 선을 그립니다.
			//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f, 0, 2.0f);

		}
			
	}
}


// 괴성 지르는 모션
void AEnemyBossBase::ServerRoarMontage_Implementation()
{
	ServerPlayMontage(RoarMontage, "None");
}

void AEnemyBossBase::MultiUpdateBossHealth_Implementation(float NewHealth, float NewMaxHealth)
{

}



void AEnemyBossBase::Attack_Implementation(FName NotifyName)
{
	if (NotifyName == FName("Attack1"))
	{
		if (!IsValid(Attack1Montage)) return;
		ServerPlayMontage(Attack1Montage, NotifyName);
	}
	else if (NotifyName == FName("Attack2"))
	{
		if (!IsValid(Attack2Montage)) return;
		ServerPlayMontage(Attack2Montage, NotifyName);
	}
	else if (NotifyName == FName("Attack3"))
	{
		if (!IsValid(Attack3Montage)) return;
		ServerPlayMontage(Attack3Montage, NotifyName);
	}
	else if (NotifyName == FName("Attack4"))
	{
		if (!IsValid(Attack4Montage)) return;
		ServerPlayMontage(Attack4Montage, NotifyName);
	}
	else if (NotifyName == FName("FWAttack1"))
	{
		if (!IsValid(FWAttack1Montage)) return;
		ServerPlayMontage(FWAttack1Montage, NotifyName);
	}
	else if (NotifyName == FName("FWAttack2"))
	{
		if (!IsValid(FWAttack2Montage)) return;
		ServerPlayMontage(FWAttack2Montage, NotifyName);
	}
	else if (NotifyName == FName("JumpAttack"))
	{
		if (!IsValid(JumpAttackMontage)) return;
		ServerPlayMontage(JumpAttackMontage, NotifyName);
	}
	else if (NotifyName == FName("BiteForward"))
	{
		if (!IsValid(BiteForwardMontage)) return;
		ServerPlayMontage(BiteForwardMontage, NotifyName);
	}
}


float AEnemyBossBase::GetCurrentHealth_Implementation()
{
	return Health;
}

float AEnemyBossBase::GetMaxHealth_Implementation()
{
	return MaxHealth;
}


bool AEnemyBossBase::IsDead_Implementation()
{
	return false;
}

void AEnemyBossBase::TakeDamage_Implementation(float Amount, AActor* DamageActor)
{
	ServerTakeDamage(Amount, this);
}

void AEnemyBossBase::ServerTakeDamage_Implementation(const float Amount, AActor* DamageActor)
{
	Health = FMath::Clamp(Health -= Amount, 0.f, MaxHealth);	// 결과값은 최소 0, 최대 100으로 설정
	FString Message = FString::Printf(TEXT("Boss APA APA"));
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, Message);
	MultiTakeDamage(Amount, this);
	// HP가 0 이하면
	if (Health <= 0)
	{
		bIsDead = true;	// 캐릭터 죽음
		ServerPlayMontage(DeathMontage, FName("Die"));
	}
	
}

void AEnemyBossBase::MulticastDie_Implementation()
{
	USkeletalMeshComponent* MyMesh = GetMesh();
	AAIController* AIController = Cast<AAIController>(GetController());
	if (!IsValid(AIController)) return;

	AAIC_EnemyBossBase* AICEnemyBossBase = Cast<AAIC_EnemyBossBase>(AIController);
	if (!IsValid(AICEnemyBossBase)) return;

	if (!IsValid(BlackboardComp))
	{
		BlackboardComp = AIController->GetBlackboardComponent();
		if (!IsValid(BlackboardComp))
		{
			UE_LOG(LogTemp, Warning, TEXT("BlackboardComponent is invalid!"));
			return;
		}
	}

	if (MyMesh)
	{
		if (HasAuthority()) {
			// 3) 모든 로직 중단
			BlackboardComp->SetValueAsEnum(FName("State"), static_cast<uint8>(E_AIState::Dead));
			BlackboardComp->ClearValue(FName("AttackTarget"));
		}
		

		// 1) 물리 시뮬레이션 켜기
		//MyMesh->SetSimulatePhysics(true);

		// 2) 충돌 모드를 Query and Physics로 설정
		//MyMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

void AEnemyBossBase::MultiTakeDamage_Implementation(const float Amount, AActor* DamageActor)
{
	OnTakeDamage.Broadcast(Health, MaxHealth);
}

UBehaviorTree* AEnemyBossBase::GetBehaviorTree()
{
	return BTTree;
}

void AEnemyBossBase::PlayRoarMontage()
{
	if (!IsValid(RoarMontage)) return;

	ServerPlayMontage(RoarMontage, FName("Roar"));
}

void AEnemyBossBase::PlayDeathMontage()
{
	if (!IsValid(DeathMontage)) return;

	ServerPlayMontage(DeathMontage, FName("Death"));
}



void AEnemyBossBase::Server_ResetBehavior_Implementation()
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (!IsValid(AIController)) return;
	
	if (AIController->RunBehaviorTree(GetBehaviorTree()))
	{
		if (!IsValid(BlackboardComp))
		{
			BlackboardComp = AIController->GetBlackboardComponent();
			if (!IsValid(BlackboardComp))
			{
				UE_LOG(LogTemp, Warning, TEXT("BlackboardComponent is invalid!"));
				return;
			}
		}

		Health = MaxHealth;
		OnTakeDamage.Broadcast(Health, MaxHealth);

		BlackboardComp->SetValueAsBool("bResetTrigger", true);
		BlackboardComp->ClearValue(FName("AttackTarget"));
		UE_LOG(LogTemp, Warning, TEXT("Success ResetTrigger"));
	}
}

void AEnemyBossBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEnemyBossBase, MaxHealth);
	DOREPLIFETIME(AEnemyBossBase, Health);
	DOREPLIFETIME(AEnemyBossBase, bIsDead);
	DOREPLIFETIME(AEnemyBossBase, bIsBossWakeupTriggered);
}

bool AEnemyBossBase::GetIsBossWakeupTriggered()
{
	return bIsBossWakeupTriggered;
}

void AEnemyBossBase::SetIsBossWakeupTriggered(bool SetBool)
{
	bIsBossWakeupTriggered = SetBool;
}

// 몽타주 행동 모음

			#pragma region Montages
void AEnemyBossBase::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// 하나의 OnAttackEnd 델리게이트로 모든 공격 종료 이벤트를 브로드캐스트
	OnBossAttackEnd.Broadcast(Montage, bInterrupted);

	// 임시
	
	
	// 종료된 몽타주에 따라 분기 처리
	if (Montage == Attack1Montage)
	{
		OnAttack1MontageEnded(bInterrupted);
	}
	else if (Montage == Attack2Montage)
	{
		OnAttack2MontageEnded(bInterrupted);
	}
	else if (Montage == Attack3Montage)
	{
		OnAttack3MontageEnded(bInterrupted);
	}
	else if (Montage == Attack4Montage)
	{
		OnAttack4MontageEnded(bInterrupted);
	}
	else if (Montage == FWAttack1Montage)
	{
		OnFW1AttackMontageEnded(bInterrupted);
	}
	else if (Montage == FWAttack2Montage)
	{
		OnFW2AttackMontageEnded(bInterrupted);
	}
	else if (Montage == JumpAttackMontage)
	{
		OnJumpAttackMontageEnded(bInterrupted);
	}
	else if (Montage == BiteForwardMontage)
	{
		OnBiteForwardMontageEnded(bInterrupted);
	}
	else if (Montage == RoarMontage)
	{
		if (bIsBossWakeupTriggered == true)
			OnWakeUp.Broadcast();
	}
	else if (Montage == DeathMontage)
	{
		OnDeathMontageEnded(bInterrupted);
	}
}

void AEnemyBossBase::OnAttack1MontageEnded(bool bInterrupted)
{
}

void AEnemyBossBase::OnAttack2MontageEnded(bool bInterrupted)
{
}

void AEnemyBossBase::OnAttack3MontageEnded(bool bInterrupted)
{
}

void AEnemyBossBase::OnAttack4MontageEnded(bool bInterrupted)
{
}

void AEnemyBossBase::OnFW1AttackMontageEnded(bool bInterrupted)
{
}

void AEnemyBossBase::OnFW2AttackMontageEnded(bool bInterrupted)
{
}

void AEnemyBossBase::OnJumpAttackMontageEnded(bool bInterrupted)
{
}

void AEnemyBossBase::OnBiteForwardMontageEnded(bool bInterrupted)
{
}

void AEnemyBossBase::OnRoarMontageEnded(bool bInterrupted)
{
}

void AEnemyBossBase::OnDeathMontageEnded(bool bInterrupted)
{
	if(HasAuthority())
		ServerDie();
}

#pragma endregion