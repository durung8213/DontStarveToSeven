// Fill out your copyright notice in the Description page of Project Settings.


#include "Hamkt/AnimNotify/BossArmCapsuleSweepNotifyState.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Hamkt/Enemy/AI/BPI_EnemyAI.h"
#include "Kimmh/BasePlayerCharacter.h"

void UBossArmCapsuleSweepNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
}

void UBossArmCapsuleSweepNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	if (!IsValid(MeshComp)) return;
	
	UWorld* World = MeshComp->GetWorld();
	if (!IsValid(World)) return;

	AActor* OwnerActor = MeshComp->GetOwner();
	if (!OwnerActor) return;

	AccumulatedTime += FrameDeltaTime;
	if (AccumulatedTime < 0.001f)	// ������ ��� ���
	{
		return;
	}

	AccumulatedTime = 0.0f; // ���� �� ���� �ð� �ʱ�ȭ

	// �������Ӹ��� �� ������ ���� ĸ�� ����
	for (auto& Segment : ArmSegments)
	{
		FVector StartPos = MeshComp->GetSocketLocation(Segment.Key);
		FVector EndPos = MeshComp->GetSocketLocation(Segment.Value);

		// ����
		PerformCapsuleSweep(World, OwnerActor, StartPos, EndPos);
	}
}

void UBossArmCapsuleSweepNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	// ������ ������ ����� �ʱ�ȭ
	AlreadyHitActors.Reset();
}

void UBossArmCapsuleSweepNotifyState::PerformCapsuleSweep(UWorld* World, AActor* OwnerActor, const FVector& StartPos, const FVector& EndPos)
{
	
	FVector TraceVec = EndPos - StartPos;	// �� ���� - ���� ���� = �� �������� ���� (����� �Ÿ�)
	float Length = TraceVec.Size();			// Size() ȣ�� �� �ش� ������ ���̸� ����.
	if (Length < KINDA_SMALL_NUMBER)
		return;	// �� ���� ���� ������ ���� ���ʿ�.
	
	// ĸ�� �߾�, ���� ���
	FVector Center = (StartPos + EndPos) * 0.5f;			// ���������� �������� ������ ������ ������ �߽ɰ��� ����
	float HalfHeight = (Length * 0.5f) + CapsuleRadius;		// (�� ������ �Ÿ��� ���ݰ�) �� ĸ���� �ݰ��� ���Ͽ� ĸ���� ��ü������ ���ݰ��� ����

	// ������� -> ĸ�� ȸ����
	FVector Dir = TraceVec.GetSafeNormal();
	FRotator CapsuleRot = FRotationMatrix::MakeFromZ(Dir).Rotator();
	FQuat CapsuleQuat = CapsuleRot.Quaternion();

	// ĸ�� ����
	FCollisionShape CapsuleShape = FCollisionShape::MakeCapsule(CapsuleRadius * 10.f, HalfHeight);

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_GameTraceChannel1);

	// Trace Params
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(OwnerActor);	// �ڱ� �ڽ� ����

	TArray<FHitResult> HitResults;
	bool bHit = World->SweepMultiByObjectType(
		HitResults,
		Center,
		Center,	// ȸ���� ���� (��ġ�̵� ����)
		CapsuleQuat,
		ObjectQueryParams,
		CapsuleShape,
		QueryParams
	);

//	// ������
//#if WITH_EDITOR
//	{
//		FColor Color = bHit ? FColor::Red : FColor::Green;
//		DrawDebugCapsule(World, Center, HalfHeight, CapsuleRadius * 10.f, CapsuleQuat, Color, false, 0.05f);
//	}
//#endif
	
	if (bHit)
	{
		for (auto& Hit : HitResults)
		{
			AActor* HitActor = Hit.GetActor();	// ��Ʈ�� ���͸� ������
			if (IsValid(HitActor) && HitActor != OwnerActor)
			{
				// �̹� ���� �����ΰ�?
				if (!AlreadyHitActors.Contains(HitActor))
				{
					// ���Ͱ� �ش� �������̽��� �����ϴ°�?
					if (HitActor->ActorHasTag(TEXT("Player"))
						&& HitActor->GetClass()->ImplementsInterface(UBPI_EnemyAI::StaticClass()))
					{
						FString Message = FString::Printf(TEXT("Boss 25F ATTACK !!!"));
						GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, Message);
						if (ABasePlayerCharacter* PlayerCharacter = Cast<ABasePlayerCharacter>(HitActor))
						{
							PlayerCharacter->SeverTakeBossDamage(25.f, OwnerActor);
						}
						//IBPI_EnemyAI::Execute_TakeDamage(HitActor, 25.f);	// HitActor �� ĳ���� TakeDamage �Լ��� �����ͼ� 25 �������� ������.
					}
				}

				// �ߺ� ������ ����
				AlreadyHitActors.Add(HitActor);
			}
		}
	}


}
