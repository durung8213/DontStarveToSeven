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

	// Pawn�� ����� AIController Ŭ������ ��������� �����մϴ�.
	AIControllerClass = AAIC_EnemyBossBase::StaticClass();

	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// ĸ�� ������Ʈ ũ�� ����
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	// 42.f: ĸ�� �ݰ�, 96.f: ĸ�� �ݳ��� (�ʿ信 ���� �� ����)

	// ���̷��� �Ž��� ��� ��ġ�� ĸ���� �°� ����
	// ���� ĳ������ �޽��� ĸ���� �߾Ӻ��� ���� �����Ƿ�, �Ʒ��� �������� �־� ĸ���� �ڿ������� ����ϴ�.
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -96.f));

	// Z�� Yaw ���� ȸ��
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	// �Ž� ������ ���� (�ʿ� �� ����)
	GetMesh()->SetWorldScale3D(FVector(3.f, 3.f, 3.f));

	// �÷��̾�� ī�޶� �浹 ����. ī�޶� Collision Ignore 
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

// ������ ������ �� ��ȯ
	// �ұ�Ģ�� �ٴ� ����
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

	// ���ϴ� ���� ��ġ (��: ������ ���� ��ġ)
	FVector SpawnLocation = GetActorLocation();

	// ���� Ʈ���̽��� �̿��ؼ� ���� �Ʒ� 500 �Ÿ����� Ž��
	FHitResult HitResult;
	FVector TraceStart = SpawnLocation;
	FVector TraceEnd = SpawnLocation - FVector(0.f, 0.f, 500.f);

	// �ٴ��� �ݸ��� ä���� ���
	if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility))
	{
		SpawnLocation = HitResult.Location;
	}
	else
	{
		// �ش� ����� ������ ��� ĸ�� ���̸� ���� ���� ������ ������� ��ȯ
		SpawnLocation.Z -= GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	}

	FRotator SpawnRotation = GetActorRotation();

	// ���� �Ķ���� ����
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Niagara ���� ����
	AActor* SpawnedNiagara = GetWorld()->SpawnActor<AActor>(NiagaraActorClass, SpawnLocation, SpawnRotation, SpawnParams);
	if (!SpawnedNiagara)
	{
		UE_LOG(LogTemp, Error, TEXT("SpawnAndDestroyNiagaraEffect: Failed to spawn NiagaraActor."));
		return;
	}

	// (�ɼ�) ���� ĳ���Ϳ� Attach: ���ϴ� ��� KeepWorldTransform �Ǵ� SnapToTarget ����
	SpawnedNiagara->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

	// 1�� �ڿ� SpawnedNiagara�� �ı��ϴ� Ÿ�̸� ����

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [SpawnedNiagara]()
		{
			if (IsValid(SpawnedNiagara))
			{
				SpawnedNiagara->Destroy();
			}
		}, 0.7f, false);
}

// JumpAttack �ൿ �� �ߵ�
void AEnemyBossBase::Landed(const FHitResult& Hit)
{
	// �θ� Ŭ����(�⺻ ���� ó��) ���� ȣ��
	Super::Landed(Hit);

	// ���⼭ MovementMode�� Walking���� ��ȯ
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->SetMovementMode(MOVE_Walking);
	}

	FString Message = FString::Printf(TEXT("Land good"));

	// �ʿ��ϴٸ�, ���� ���� ���� ó���� �ϰų�
	// ��ġ�� �̼� ���� (��: ������Ʈ ���� ���� ���� ���)

	ServerDamageNearByActorsOnLanding();

	// BP_SandPack ��ȯ �� 1�� �� �ı�
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
	// 1. ���� ������ ��������, ��ü ���� ���� ���͸� �˻� - RootComponent ����
	FVector Center = GetActorLocation();
	float SphereRadius = 250.f * GetMesh()->GetComponentScale().X;	// ���� ���� �������� ������ ����� ����

	TArray<FHitResult> HitResults;	// Hit�� ����� TArray �迭�� ��ƿ�
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_GameTraceChannel1);	// �÷��̾�

	FCollisionShape Sphere = FCollisionShape::MakeSphere(SphereRadius);

	// Ʈ���̽� �Ķ����
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);	// �ڱ� �ڽ��� ����

	bool bHit = GetWorld()->SweepMultiByObjectType(
		HitResults,
		Center,
		Center,
		FQuat::Identity,
		ObjectQueryParams,
		Sphere,
		QueryParams
	);

	// ��Ʈ�� ����� �ִٸ�.
	if (bHit)
	{
		for (auto& HitResult : HitResults)
		{
			AActor* HitActor = HitResult.GetActor();
			if (IsValid(HitActor))	// ���� ��ȿ�ϴٸ�
			{
				// ���Ͱ� �ش� �������̽��� �����ϴ°�?
				if (HitActor->ActorHasTag(TEXT("Player"))
					&& HitActor->GetClass()->ImplementsInterface(UBPI_EnemyAI::StaticClass()))
				{

					FString Message = FString::Printf(TEXT("Boss 25F ATTACK !!!"));
					GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, Message);
					IBPI_EnemyAI::Execute_TakeDamage(HitActor, 25.f, this);	// HitActor �� ĳ���� TakeDamage �Լ��� �����ͼ� 25 �������� ������.

				}
			}
		}
	}

	// ����� ��ü �ð�ȭ
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
	// GetSkeletalMeshComponent() �Ǵ� Mesh ������ AnimInstance�� ������
	USkeletalMeshComponent* SkeletalMeshComp = GetMesh();

	// AttackMontage�� �Ҵ� �Ǿ��ִ°�?
	if (Montage)
	{
		// ĳ������ ���̷�Ż �޽ÿ��� �ִ� �ν��Ͻ��� ������
		UAnimInstance* AnimInstance = SkeletalMeshComp->GetAnimInstance();
		float PlayRate = 1.2f;	// ��Ÿ�� ����ӵ�
		if (AnimInstance && Montage)	// �ִ��ν��Ͻ��� ��ȿ�ϸ�
		{
			// ���� ��Ÿ�ָ� ���,   ��� ������ Duration�� 0���� ū �� ��ȯ
			float Duration = AnimInstance->Montage_Play(Montage, PlayRate);
			if (Duration > 0.f)
			{
				// MontageEndedDelegate�� ���� �ִϸ��̼�(��Ÿ��) ����� ���� �������� �ݹ����� �� �� ����
				FOnMontageEnded MontageEndedDelegate;
				MontageEndedDelegate.BindUObject(this, &AEnemyBossBase::OnMontageEnded);
				AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, Montage);	// AnimBP��
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("AttackMontage is not set on %s"), *GetName());
		}
	}

	if (HasAuthority())
	{
		// ��Ƽ���� �̸��� Attack���
		if (NotifyName == "Attack")
		{
			// **	 SweepSingleByObjectType  ==  SphereTraceForObjects		**//


			// 1. ���۰� �� ��ġ & ��ü ũ��
			FVector Start = GetActorLocation();
			FVector End = Start + GetActorForwardVector() * 200.f;
			float SphereRadius = 25.f;	// ��ü ũ��

			// 2. ����� ������ ����
			FHitResult HitResult;

			// 3. Pawn Ÿ�� ��ü ������� �˻�
			FCollisionObjectQueryParams ObjectQueryParams;
			ObjectQueryParams.AddObjectTypesToQuery(ECC_GameTraceChannel1);

			// 4. Ʈ���̽��Ķ���� (�ڱ��ڽ� ����).. Self Ignore�� ���� ���
			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(this);

			// 5. Sphere Trace ����
			bool bHit = GetWorld()->SweepSingleByObjectType(
				HitResult,		// ����� ������ ����
				Start,			// ������
				End,			// ����
				FQuat::Identity,	// ȸ���� (ȸ�� 0)
				ObjectQueryParams,	// ������ ��ü Ÿ�Ե�
				FCollisionShape::MakeSphere(SphereRadius),	// ���� �ݸ��� ����
				QueryParams			// SelfIgnore�� ���� ���
			);

			// 6. ���ó��
				// Trace�� ��Ʈ��
			if (bHit)
			{
				// �浹�� ����
				AActor* HitActor = HitResult.GetActor();

				if (!IsValid(HitActor)) return;


				// ���Ͱ� �ش� �������̽��� �����ϴ°�?
				if (HitActor->ActorHasTag(TEXT("Player"))
					&& HitActor->GetClass()->ImplementsInterface(UBPI_EnemyAI::StaticClass()))
				{
					FString Message = FString::Printf(TEXT("Boss 25F ATTACK !!!"));
					GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, Message);
					IBPI_EnemyAI::Execute_TakeDamage(HitActor, 25.f, this);	// HitActor �� ĳ���� TakeDamage �Լ��� �����ͼ� 25 �������� ������.
				}
			}

			// �������� ������ ��ü�� �׸��ϴ�.
			//DrawDebugSphere(GetWorld(), Start, SphereRadius, 12, FColor::Green, false, 2.0f, 0, 2.0f);
			//DrawDebugSphere(GetWorld(), End, SphereRadius, 12, FColor::Green, false, 2.0f, 0, 2.0f);

			// �� ���� �մ� ���� �׸��ϴ�.
			//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f, 0, 2.0f);

		}
			
	}
}


// ���� ������ ���
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
	Health = FMath::Clamp(Health -= Amount, 0.f, MaxHealth);	// ������� �ּ� 0, �ִ� 100���� ����
	FString Message = FString::Printf(TEXT("Boss APA APA"));
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, Message);
	MultiTakeDamage(Amount, this);
	// HP�� 0 ���ϸ�
	if (Health <= 0)
	{
		bIsDead = true;	// ĳ���� ����
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
			// 3) ��� ���� �ߴ�
			BlackboardComp->SetValueAsEnum(FName("State"), static_cast<uint8>(E_AIState::Dead));
			BlackboardComp->ClearValue(FName("AttackTarget"));
		}
		

		// 1) ���� �ùķ��̼� �ѱ�
		//MyMesh->SetSimulatePhysics(true);

		// 2) �浹 ��带 Query and Physics�� ����
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

// ��Ÿ�� �ൿ ����

			#pragma region Montages
void AEnemyBossBase::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// �ϳ��� OnAttackEnd ��������Ʈ�� ��� ���� ���� �̺�Ʈ�� ��ε�ĳ��Ʈ
	OnBossAttackEnd.Broadcast(Montage, bInterrupted);

	// �ӽ�
	
	
	// ����� ��Ÿ�ֿ� ���� �б� ó��
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