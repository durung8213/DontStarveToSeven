// Fill out your copyright notice in the Description page of Project Settings.


#include "Leesh/SHWeaponItem.h"
#include "Components/CapsuleComponent.h"
#include "Hamkt/Enemy/AI/BPI_EnemyAI.h"


ASHWeaponItem::ASHWeaponItem()
{
	HitCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitCollision"));
	HitCollision->SetupAttachment(RootComponent);
}

void ASHWeaponItem::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority()) {
		// �������� ����ǰ�
		if (OtherActor != GetParentActor()) {
			// ������ �Ȱ��� �θ� ���Ͱ� �ƴϰ�
			if (bCanAttack) {
				// ������ ������ ���°�
				if (!HitActorArr.Contains(OtherActor)) {
					// ������ �� ���Ͱ� ���� ���� �迭�� ������
					IBPI_EnemyAI* Interface = Cast<IBPI_EnemyAI>(OtherActor);
					if (Interface) {
						// �������̽��� ������ ������ ��
						Interface->Execute_TakeDamage(OtherActor, 30.f, GetParentActor());
					}
					HitActorArr.Add(OtherActor);
				}
			}
		}
	}
	
}

void ASHWeaponItem::BeginPlay()
{
	Super::BeginPlay();
	HitCollision->OnComponentBeginOverlap.AddDynamic(this, &ASHWeaponItem::OnCapsuleBeginOverlap);
}

void ASHWeaponItem::AttackStart()
{
	bCanAttack = true;
}

void ASHWeaponItem::AttackStop()
{
	bCanAttack = false;
	HitActorArr.Empty();
}

void ASHWeaponItem::SetCollisionObjectTypeWeapon()
{
	Super::SetCollisionObjectTypeWeapon();
	HitCollision->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel7);
}

