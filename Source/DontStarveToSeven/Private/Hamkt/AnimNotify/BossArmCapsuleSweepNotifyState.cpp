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
	if (AccumulatedTime < 0.001f)	// 프레임 드랍 대비
	{
		return;
	}

	AccumulatedTime = 0.0f; // 실행 후 누적 시간 초기화

	// 매프레임마다 각 구간에 대한 캡슐 스윕
	for (auto& Segment : ArmSegments)
	{
		FVector StartPos = MeshComp->GetSocketLocation(Segment.Key);
		FVector EndPos = MeshComp->GetSocketLocation(Segment.Value);

		// 스윕
		PerformCapsuleSweep(World, OwnerActor, StartPos, EndPos);
	}
}

void UBossArmCapsuleSweepNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	// 공격이 끝나면 목록을 초기화
	AlreadyHitActors.Reset();
}

void UBossArmCapsuleSweepNotifyState::PerformCapsuleSweep(UWorld* World, AActor* OwnerActor, const FVector& StartPos, const FVector& EndPos)
{
	
	FVector TraceVec = EndPos - StartPos;	// 끝 지점 - 시작 지점 = 두 점사이의 벡터 (방향과 거리)
	float Length = TraceVec.Size();			// Size() 호출 시 해당 벡터의 길이를 구함.
	if (Length < KINDA_SMALL_NUMBER)
		return;	// 두 점이 거의 같으면 스윕 불필요.
	
	// 캡슐 중앙, 길이 계산
	FVector Center = (StartPos + EndPos) * 0.5f;			// 시작지점과 끝지점을 더한후 반으로 나누면 중심값이 나옴
	float HalfHeight = (Length * 0.5f) + CapsuleRadius;		// (두 점사의 거리의 절반값) 과 캡슐의 반경을 더하여 캡슐의 전체높이의 절반값을 구함

	// 진행방향 -> 캡슐 회전값
	FVector Dir = TraceVec.GetSafeNormal();
	FRotator CapsuleRot = FRotationMatrix::MakeFromZ(Dir).Rotator();
	FQuat CapsuleQuat = CapsuleRot.Quaternion();

	// 캡슐 형상
	FCollisionShape CapsuleShape = FCollisionShape::MakeCapsule(CapsuleRadius * 10.f, HalfHeight);

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_GameTraceChannel1);

	// Trace Params
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(OwnerActor);	// 자기 자신 무시

	TArray<FHitResult> HitResults;
	bool bHit = World->SweepMultiByObjectType(
		HitResults,
		Center,
		Center,	// 회전만 적용 (위치이동 없음)
		CapsuleQuat,
		ObjectQueryParams,
		CapsuleShape,
		QueryParams
	);

//	// 디버깅용
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
			AActor* HitActor = Hit.GetActor();	// 히트한 액터를 가져옴
			if (IsValid(HitActor) && HitActor != OwnerActor)
			{
				// 이미 맞은 액터인가?
				if (!AlreadyHitActors.Contains(HitActor))
				{
					// 액터가 해당 인터페이스를 구현하는가?
					if (HitActor->ActorHasTag(TEXT("Player"))
						&& HitActor->GetClass()->ImplementsInterface(UBPI_EnemyAI::StaticClass()))
					{
						FString Message = FString::Printf(TEXT("Boss 25F ATTACK !!!"));
						GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, Message);
						if (ABasePlayerCharacter* PlayerCharacter = Cast<ABasePlayerCharacter>(HitActor))
						{
							PlayerCharacter->SeverTakeBossDamage(25.f, OwnerActor);
						}
						//IBPI_EnemyAI::Execute_TakeDamage(HitActor, 25.f);	// HitActor 된 캐릭의 TakeDamage 함수를 가져와서 25 데미지를 입힌다.
					}
				}

				// 중복 데미지 방지
				AlreadyHitActors.Add(HitActor);
			}
		}
	}


}
