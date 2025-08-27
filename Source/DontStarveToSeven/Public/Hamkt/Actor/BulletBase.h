// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "BulletBase.generated.h"

UCLASS()
class DONTSTARVETOSEVEN_API ABulletBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABulletBase();

	UFUNCTION(NetMulticast, Reliable)
	virtual void Multicast_SpawnExplosionEffect();

protected:
	virtual void BeginPlay() override;

	// 충돌 이벤트 처리 함수
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse,
		const FHitResult& Hit);

	// 충돌 처리를 위한 컴포넌트 ( 포탄의 충돌 영역 )
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* CollisionComp;

	// Mesh Component : 발사체의 외형
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = "Components")
	UStaticMeshComponent* MeshComp;

	// 발사체의 무브먼트 컴포넌트
	UPROPERTY()
	UProjectileMovementComponent* ProjectileMovement;

	// 발사체가 입히는 피해량
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float Damage;

	// 발사체가 이동하는 최대 거리 ( 여기서는 유닛 단위 )
	UPROPERTY(EditDefaultsOnly, Category = "Lifetime")
	float MaxDistance;

	// 발사체에 붙을 이미지
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UParticleSystemComponent* ParticleSystemComp;

		// 포탄 충돌 파티클
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UParticleSystem* ExplosionParticle;

public:

	

};
