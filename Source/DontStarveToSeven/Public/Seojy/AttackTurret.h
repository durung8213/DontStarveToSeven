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
	//생성자
	AAttackTurret();
	~AAttackTurret();

	//적을 감지했을 때, 실행될 함수 (Overlap)
	UFUNCTION()
	void OnEnemyDetected(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	// Overlap 이벤트 : 적이 감지 영역에서 나갔을 때 호출
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


	//주변 적 감지 및 목표 업데이트
	void UpdateTarget();

	// 일정 주기로 적 감지 배열을 재검사
	UFUNCTION()
	void PeriodicTargetCheck();

	// 터렛 헤드 회전 보간 처리
	void UpdateRotation(float DeltaTime);

	// 포탄 발사
	void TryFire();

	// 서버에서 실제 발사 로직.
	UFUNCTION(Server, Reliable)
	void ServerFire();

	//// 서버에서 TurretHead 회전 계산
	//UFUNCTION(Server, Reliable)
	//void ServerComputeRotation(float DeltaTime);

	
	// 상태 변경 시, 클라이언트에서 호출
	UFUNCTION()
	void OnRep_TurretState();

	//포탄 생성 함수
	UFUNCTION()
	void SpawnProjectile();

	UFUNCTION()
	void OnRep_TurretHeadRot();

public:
	// 발사 관련 설정
	UPROPERTY(EditDefaultsOnly, Category = "Turret")
	TSubclassOf<ABullet> BulletClass;
	
	UPROPERTY(ReplicatedUsing = OnRep_TurretHeadRot) 
	FRotator TurretHeadRot;


protected:
	// 적을 감지하기 위한 스피어 컴포넌트 선언
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class USphereComponent* DetectionSphere;

	// 터렛의 루트 컴포넌트
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = "Mesh")
	USceneComponent* RootComp;

	//터렛의 스태틱 메쉬 에셋
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = "Mesh")
	UStaticMeshComponent* TurretBase;

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = "Mesh")
	UStaticMeshComponent* TurretHead;
	
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = "Mesh")
	UStaticMeshComponent* TurretSwivel;

	// 터렛 상태( 서버에서 결정 )
	UPROPERTY(ReplicatedUsing = OnRep_TurretState, BlueprintReadOnly, Category = "Turret")
	bool bIsInAttackMode;

	// 현재 타겟 ( 적 )
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Turret")
	AActor* CurrentTarget;

	// 기본 순찰 시 회전값 ( 적 없을 때 복귀 )
	UPROPERTY(EditDefaultsOnly, Category = "Turret")
	FRotator DefaultRotation;


	UPROPERTY(EditDefaultsOnly, Category = "Turret")
	float FireRate = 2.0f;		//초당 발사 간격

	// 적을 담아 놓을 배열
	UPROPERTY()
	TArray<AActor*> Targets;

	//// 탄환 위치 Arrow
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	//UArrowComponent* MuzzleArrow;

	// 타이머 변수
	FTimerHandle TimerHandle_Fire;
	FTimerHandle TimerHandle_TargetCheck;

	bool bIsCanFire = false;
};
