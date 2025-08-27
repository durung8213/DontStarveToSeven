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

	// �浹 �̺�Ʈ ó�� �Լ�
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse,
		const FHitResult& Hit);

	// �浹 ó���� ���� ������Ʈ ( ��ź�� �浹 ���� )
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* CollisionComp;

	// Mesh Component : �߻�ü�� ����
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = "Components")
	UStaticMeshComponent* MeshComp;

	// �߻�ü�� �����Ʈ ������Ʈ
	UPROPERTY()
	UProjectileMovementComponent* ProjectileMovement;

	// �߻�ü�� ������ ���ط�
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float Damage;

	// �߻�ü�� �̵��ϴ� �ִ� �Ÿ� ( ���⼭�� ���� ���� )
	UPROPERTY(EditDefaultsOnly, Category = "Lifetime")
	float MaxDistance;

	// �߻�ü�� ���� �̹���
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UParticleSystemComponent* ParticleSystemComp;

		// ��ź �浹 ��ƼŬ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UParticleSystem* ExplosionParticle;

public:

	

};
