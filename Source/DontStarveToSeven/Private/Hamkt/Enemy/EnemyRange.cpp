// Fill out your copyright notice in the Description page of Project Settings.


#include "Hamkt/Enemy/EnemyRange.h"
#include "Engine/World.h"						// ���� �׼���, ������
#include "GameFramework/Actor.h"				// ���� ���� �Ķ����
#include "Components/SkeletalMeshComponent.h"	// ������Ʈ Attach
#include "AIController.h"
#include "Hamkt/Enemy/AI/AIC_EnemyBase.h"
#include "GameFramework/CharacterMovementComponent.h"	// GetCharacterMovement() ��ȯŸ���� ����ϱ� ���� �ʼ�
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AEnemyRange::AEnemyRange()
{
}


void AEnemyRange::BeginPlay()
{
	Super::BeginPlay();
}

// ĳ���� ��Ȳ�� �̵��ӵ�
void AEnemyRange::SetMovementSpeed_Implementation(E_MovementSpeed NewSpeed)
{
	float SpeedValue = 0.f;	// �̵��ӵ� ��

	// �������� ���� �̵��ӵ� ���� ����
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

	// CharacterMovementComponent�� ���� MakWalkSpeed ����
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	if (!IsValid(MoveComp)) return;		// ��ȿ�� ����
	MoveComp->MaxWalkSpeed = SpeedValue;
	
}

// �ּ�&�ִ� �̵�&���ݰŸ�
void AEnemyRange::GetIdealRange_Implementation(float& AttackRadius, float& DefendRadius) const
{
	AttackRadius = 600.f;
	DefendRadius = 600.f;
}

///////////////////////////////////////////////////////////////////////////
// 
// ���� �������̽� �������Լ�
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
	// GetSkeletalMeshComponent() �Ǵ� Mesh ������ AnimInstance�� ������
	USkeletalMeshComponent* SkeletalMeshComp = GetMesh();
	if (!IsValid(AttackMontage)) return;		// EquipMontage�� ��ȿ���� ������ ����

	// ĳ������ ���̷�Ż �޽ÿ��� �ִ� �ν��Ͻ��� ������
	UAnimInstance* AnimInstance = SkeletalMeshComp->GetAnimInstance();
	if (!IsValid(AnimInstance)) return;		// �ִ��ν��Ͻ��� ��ȿ���� ������ ����

	float PlayRate = 1.2f;	// ��Ÿ�� ����ӵ�


	// ���� ��Ÿ�ָ� ���,  ��� ������ Duration�� 0���� ū �� ��ȯ
	float Duration = AnimInstance->Montage_Play(AttackMontage, PlayRate);
	if (Duration > 0.f)
	{
		// Montage ���� ��������Ʈ ����
		FOnMontageEnded MontageEndedDelegate;
		MontageEndedDelegate.BindUObject(this, &AEnemyRange::OnMontageEnded);
		AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, AttackMontage);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AttackMontage Montage not played properly."));
	}

	//////
	// ��Ƽ���� �̸��� Attack �̶��
	if (NotifyName == "Attack")
	{
		if (!IsValid(WeaponClass)) return;	// WeaponClass�� �Ҵ���� �ʾҴٸ� ����ó��

		AAIController* AIController = Cast<AAIController>(GetController());	// Self - GetAIController �� ����
		if (!IsValid(AIController)) return;

		AAIC_EnemyBase* EnemyAIController = Cast<AAIC_EnemyBase>(AIController);	// AI Controller�� ����ȯ
		if (!IsValid(EnemyAIController)) return;

		AActor* Target = EnemyAIController->GetAttackTargetActor();	// ���� Ÿ�� Ű���� ������
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

		// ���� ���� & ���Ͽ� ����ġ
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// ���� ����ü�� Ʈ�������� �ش� ������ Ʈ�������� �������� ����
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
			// UINTERFACE ����� �������̽� �Լ��� IBPI_EnemyAI::Execute_GetIdealRange()�� ����ؾ� ��
			IBPI_EnemyAI::Execute_ThrowWeaponAtTarget(this, WeaponActor, Target);
		}

		
	}
}


///////////////////////////////////////////////////////////////////////////
// 

// ������ (�߻�ü ����, Ÿ�پ���)
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

	// 1) ���Ͽ��� �и� (Detach)
	InWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	// 2) ���⿡ ProjectileMovementComponent ã��
	UProjectileMovementComponent* ProjComp = InWeapon->FindComponentByClass<UProjectileMovementComponent>();
	if (!ProjComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("WeaponActor has no ProjectileMovementComponent!"));
		return;
	}

	// 3) ������ / ��ǥ�� ���
	FVector StartLoc = InWeapon->GetActorLocation();
	FVector EndLoc = InTarget->GetActorLocation();

	// 4) SuggestProjectileVelocity �Ķ���� ����
	float LaunchSpeed = 1000.f;           // �ʱ� �ӵ� (900 cm/s)
	bool bFavorHighArc = true;           // ���� ������ �ַ���� �켱 ����
	float OverrideGravityZ = 300.f;        // �⺻ �߷º��� ���� ��(��: 700 cm/s��)�� �����Ͽ� ��ũ�� ����
	float CollisionRadius = 0.f;         // �浹 �������� ������� ����

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
		// 5) ���� �ӵ��� ProjectileMovementComponent�� ����
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