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
		// 서버에서 실행되고
		if (OtherActor != GetParentActor()) {
			// 오버랩 된것이 부모 액터가 아니고
			if (bCanAttack) {
				// 공격이 가능한 상태고
				if (!HitActorArr.Contains(OtherActor)) {
					// 오버랩 된 액터가 맞은 액터 배열에 없으며
					IBPI_EnemyAI* Interface = Cast<IBPI_EnemyAI>(OtherActor);
					if (Interface) {
						// 인터페이스가 있으면 데미지 줌
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

