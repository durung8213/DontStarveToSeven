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

	// �浹 �̺�Ʈ ó�� �Լ�
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse,
		const FHitResult& Hit);

public:	

	// �浹 ó���� ���� ������Ʈ ( ��ź�� �浹 ���� )
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* CollisionComp;

	// Mesh Component : ��ź�� ����
	UPROPERTY(VisibleAnywhere,BluePrintReadWrite, Category = "Components")
	UStaticMeshComponent* MeshComp1;

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = "Components")
	UStaticMeshComponent* MeshComp2;

	// ��ź�� �����Ʈ ������Ʈ : ��ź�� �̵��� ���
	UPROPERTY()
	UProjectileMovementComponent* ProjectileMovement;

	// ��ź�� ������ ���ط�
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float Damage;

	// ��ź�� �̵��� �� �ִ� �ִ� �Ÿ� ( ���⼭�� ���� ���� )
	UPROPERTY(EditDefaultsOnly, Category = "Lifetime")
	float MaxDistance;

	// ��ź�� ������ ��ġ(�̵� �Ÿ��� �����ϴ� ����)
	FVector SpawnLocation;

	// ��ź �浹 ��ƼŬ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effects")
	UNiagaraSystem* ExplosionEffect;
	
};
