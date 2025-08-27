// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Seojy/BaseCraft.h"
#include "Seojy/Turret/Bullet.h"
#include "Components/ArrowComponent.h"
#include "AttackTurret.generated.h"

/**
 * 
 */

class UStaticMeshComponent;

UCLASS(Blueprintable)
class DONTSTARVETOSEVEN_API AAttackTurret : public ABaseCraft
{
	GENERATED_BODY()
public:
	//������
	AAttackTurret();
	~AAttackTurret();

	//���� �������� ��, ����� �Լ� (Overlap)
	UFUNCTION()
	void OnEnemyDetected(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	// Overlap �̺�Ʈ : ���� ���� �������� ������ �� ȣ��
	UFUNCTION()
	void OnEnemyLost(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);
	virtual void SetCraftMode(ECraftMode NewMode) override;
	
	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	//�ֺ� �� ���� �� ��ǥ ������Ʈ
	void UpdateTarget();

	// ���� �ֱ�� �� ���� �迭�� ��˻�
	UFUNCTION()
	void PeriodicTargetCheck();

	// �ͷ� ��� ȸ�� ���� ó��
	void UpdateRotation(float DeltaTime);

	// ��ź �߻�
	void TryFire();

	// �������� ���� �߻� ����.
	UFUNCTION(Server, Reliable)
	void ServerFire();

	//// �������� TurretHead ȸ�� ���
	//UFUNCTION(Server, Reliable)
	//void ServerComputeRotation(float DeltaTime);

	
	// ���� ���� ��, Ŭ���̾�Ʈ���� ȣ��
	UFUNCTION()
	void OnRep_TurretState();

	//��ź ���� �Լ�
	UFUNCTION()
	void SpawnProjectile();

	UFUNCTION()
	void OnRep_TurretHeadRot();

public:
	// �߻� ���� ����
	UPROPERTY(EditDefaultsOnly, Category = "Turret")
	TSubclassOf<ABullet> BulletClass;
	
	UPROPERTY(ReplicatedUsing = OnRep_TurretHeadRot) 
	FRotator TurretHeadRot;


protected:
	// ���� �����ϱ� ���� ���Ǿ� ������Ʈ ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class USphereComponent* DetectionSphere;

	// �ͷ��� ��Ʈ ������Ʈ
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = "Mesh")
	USceneComponent* RootComp;

	//�ͷ��� ����ƽ �޽� ����
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = "Mesh")
	UStaticMeshComponent* TurretBase;

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = "Mesh")
	UStaticMeshComponent* TurretHead;
	
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = "Mesh")
	UStaticMeshComponent* TurretSwivel;

	// �ͷ� ����( �������� ���� )
	UPROPERTY(ReplicatedUsing = OnRep_TurretState, BlueprintReadOnly, Category = "Turret")
	bool bIsInAttackMode;

	// ���� Ÿ�� ( �� )
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Turret")
	AActor* CurrentTarget;

	// �⺻ ���� �� ȸ���� ( �� ���� �� ���� )
	UPROPERTY(EditDefaultsOnly, Category = "Turret")
	FRotator DefaultRotation;


	UPROPERTY(EditDefaultsOnly, Category = "Turret")
	float FireRate = 2.0f;		//�ʴ� �߻� ����

	// ���� ��� ���� �迭
	UPROPERTY()
	TArray<AActor*> Targets;

	//// źȯ ��ġ Arrow
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	//UArrowComponent* MuzzleArrow;

	// Ÿ�̸� ����
	FTimerHandle TimerHandle_Fire;
	FTimerHandle TimerHandle_TargetCheck;

	bool bIsCanFire = false;
};
