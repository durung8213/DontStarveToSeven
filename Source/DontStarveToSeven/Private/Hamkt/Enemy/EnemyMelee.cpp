// Fill out your copyright notice in the Description page of Project Settings.


#include "Hamkt/Enemy/EnemyMelee.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "CollisionQueryParams.h"
#include "Net/UnrealNetwork.h"
#include "Hamkt/Enemy/AI/AIC_EnemyBase.h"

AEnemyMelee::AEnemyMelee()
{

}

void AEnemyMelee::BeginPlay()
{
	Super::BeginPlay();
}

// �������� / SphereTraceForObjects
void AEnemyMelee::Attack_Implementation(FName NotifyName)
{
	ServerAttackMontage(NotifyName);
}

void AEnemyMelee::ServerAttackMontage_Implementation(const FName& NotifyName)
{
	MulticastAttackMontage(NotifyName);
}

void AEnemyMelee::MulticastAttackMontage_Implementation(const FName& NotifyName)
{
	// GetSkeletalMeshComponent() �Ǵ� Mesh ������ AnimInstance�� ������
	USkeletalMeshComponent* SkeletalMeshComp = GetMesh();

	// AttackMontage�� �Ҵ� �Ǿ��ִ°�?
	if (AttackMontage)
	{
		// ĳ������ ���̷�Ż �޽ÿ��� �ִ� �ν��Ͻ��� ������
		UAnimInstance* AnimInstance = SkeletalMeshComp->GetAnimInstance();
		float PlayRate = 1.2f;	// ��Ÿ�� ����ӵ�
		if (AnimInstance && AttackMontage)	// �ִ��ν��Ͻ��� ��ȿ�ϸ�
		{
			// ���� ��Ÿ�ָ� ���,   ��� ������ Duration�� 0���� ū �� ��ȯ
			float Duration = AnimInstance->Montage_Play(AttackMontage, PlayRate);
			if (Duration > 0.f)
			{
				// MontageEndedDelegate�� ���� �ִϸ��̼�(��Ÿ��) ����� ���� �������� �ݹ����� �� �� ����
				FOnMontageEnded MontageEndedDelegate;
				MontageEndedDelegate.BindUObject(this, &AEnemyMelee::OnMontageEnded);
				AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, AttackMontage);	// AnimBP��
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
			ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

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
				if (	(HitActor->ActorHasTag(TEXT("Player")) || HitActor->ActorHasTag(TEXT("Craft"))) 
					  && HitActor->GetClass()->ImplementsInterface(UBPI_EnemyAI::StaticClass()))
				{
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

