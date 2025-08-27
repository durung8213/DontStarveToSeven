// Fill out your copyright notice in the Description page of Project Settings.


#include "Hamkt/Actor/WeaponSphere.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Hamkt/Enemy/AI/BPI_EnemyAI.h"


AWeaponSphere::AWeaponSphere()
{
	////// Sphere Collision ������Ʈ�� ����
	//SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	//SphereCollision->SetupAttachment(StaticMeshComp);
	//
	////// ProjectileMovement ����
	//ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	//ProjectileMovement->bRotationFollowsVelocity = true;  // ����ü ���� �������� ȸ��

	//SphereCollision->OnComponentHit.AddDynamic(this, &AWeaponSphere::OnSphereHit);
}

// ���� �Ѿ��� ���ʹ����̶� ��ĥ��� ������ ����
//void AWeaponSphere::OnSphereHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
//{
//	// �ڽ�(self)���� �浹�� �����մϴ�.
//	if (!OtherActor || OtherActor == this)
//	{
//		return; StaticMeshComp
//	}
//
//
//	// ���Ͱ� �ش� �������̽��� �����ϴ°�?
//	if (OtherActor->GetClass()->ImplementsInterface(UBPI_EnemyAI::StaticClass()))
//	{
//		IBPI_EnemyAI::Execute_TakeDamage(OtherActor, 25.f);	// HitActor �� ĳ���� TakeDamage �Լ��� �����ͼ� 25 �������� ������.
//
//		UE_LOG(LogTemp, Warning, TEXT("Hit by %s. Destroying self."), *OtherActor->GetName());
//
//		// �ڱ� �ڽ�(this)�� �ı�
//		Destroy();
//	}
//}
