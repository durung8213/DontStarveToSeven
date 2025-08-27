// Fill out your copyright notice in the Description page of Project Settings.


#include "Hamkt/Actor/WeaponSphere.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Hamkt/Enemy/AI/BPI_EnemyAI.h"


AWeaponSphere::AWeaponSphere()
{
	////// Sphere Collision 컴포넌트를 생성
	//SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	//SphereCollision->SetupAttachment(StaticMeshComp);
	//
	////// ProjectileMovement 생성
	//ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	//ProjectileMovement->bRotationFollowsVelocity = true;  // 투사체 진행 방향으로 회전

	//SphereCollision->OnComponentHit.AddDynamic(this, &AWeaponSphere::OnSphereHit);
}

// 현재 총알이 에너미폰이랑 겹칠까봐 무시인 상태
//void AWeaponSphere::OnSphereHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
//{
//	// 자신(self)과의 충돌은 무시합니다.
//	if (!OtherActor || OtherActor == this)
//	{
//		return; StaticMeshComp
//	}
//
//
//	// 액터가 해당 인터페이스를 구현하는가?
//	if (OtherActor->GetClass()->ImplementsInterface(UBPI_EnemyAI::StaticClass()))
//	{
//		IBPI_EnemyAI::Execute_TakeDamage(OtherActor, 25.f);	// HitActor 된 캐릭의 TakeDamage 함수를 가져와서 25 데미지를 입힌다.
//
//		UE_LOG(LogTemp, Warning, TEXT("Hit by %s. Destroying self."), *OtherActor->GetName());
//
//		// 자기 자신(this)을 파괴
//		Destroy();
//	}
//}
