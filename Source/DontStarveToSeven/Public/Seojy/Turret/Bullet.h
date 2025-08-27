// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraSystem.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Bullet.generated.h"

class UStaticMeshComopnent;
class UProjectileMovementComponent;

UCLASS()
class DONTSTARVETOSEVEN_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:
	ABullet();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SpawnExplosionEffect();

protected:
	virtual void BeginPlay() override;

	// 충돌 이벤트 처리 함수
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse,
		const FHitResult& Hit);

public:	

	// 충돌 처리를 위한 컴포넌트 ( 포탄의 충돌 영역 )
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* CollisionComp;

	// Mesh Component : 포탄의 외형
	UPROPERTY(VisibleAnywhere,BluePrintReadWrite, Category = "Components")
	UStaticMeshComponent* MeshComp1;

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = "Components")
	UStaticMeshComponent* MeshComp2;

	// 포탄의 무브먼트 컴포넌트 : 포탄의 이동을 담당
	UPROPERTY()
	UProjectileMovementComponent* ProjectileMovement;

	// 포탄이 입히는 피해량
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float Damage;

	// 포탄이 이동할 수 있는 최대 거리 ( 여기서는 유닛 단위 )
	UPROPERTY(EditDefaultsOnly, Category = "Lifetime")
	float MaxDistance;

	// 포탄이 생성된 위치(이동 거리를 측정하는 기준)
	FVector SpawnLocation;

	// 포탄 충돌 파티클
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effects")
	UNiagaraSystem* ExplosionEffect;
	
};
