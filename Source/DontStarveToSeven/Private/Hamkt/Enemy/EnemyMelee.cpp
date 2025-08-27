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

// 근접공격 / SphereTraceForObjects
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
	// GetSkeletalMeshComponent() 또는 Mesh 등으로 AnimInstance를 가져옴
	USkeletalMeshComponent* SkeletalMeshComp = GetMesh();

	// AttackMontage가 할당 되어있는가?
	if (AttackMontage)
	{
		// 캐릭터의 스켈레탈 메시에서 애님 인스턴스를 가져옴
		UAnimInstance* AnimInstance = SkeletalMeshComp->GetAnimInstance();
		float PlayRate = 1.2f;	// 몽타주 재생속도
		if (AnimInstance && AttackMontage)	// 애님인스턴스가 유효하면
		{
			// 공격 몽타주를 재생,   재생 성공시 Duration은 0보다 큰 값 반환
			float Duration = AnimInstance->Montage_Play(AttackMontage, PlayRate);
			if (Duration > 0.f)
			{
				// MontageEndedDelegate를 통해 애니메이션(몽타주) 재생이 언제 끝났는지 콜백으로 알 수 있음
				FOnMontageEnded MontageEndedDelegate;
				MontageEndedDelegate.BindUObject(this, &AEnemyMelee::OnMontageEnded);
				AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, AttackMontage);	// AnimBP쪽
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
			ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

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
				if (	(HitActor->ActorHasTag(TEXT("Player")) || HitActor->ActorHasTag(TEXT("Craft"))) 
					  && HitActor->GetClass()->ImplementsInterface(UBPI_EnemyAI::StaticClass()))
				{
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

