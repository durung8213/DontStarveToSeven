// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Hamkt/Enemy/AI/BPI_EnemyAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyBossBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWakeUp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTakeDamage, float, InCurrentHealth, float, InMaxHealth);
// 델리게이트 선언: 두 개의 매개변수를 받음 (종료된 몽타주와 중단 여부)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBossAttackEndDelegate, UAnimMontage*, Montage, bool, bInterrupted);


class AAIC_EnemyBossBase;
class UBehaviorTree;
class ABossZoneBase;

UCLASS()
class DONTSTARVETOSEVEN_API AEnemyBossBase : public ACharacter, public IBPI_EnemyAI
{
	GENERATED_BODY()

public:
	AEnemyBossBase();

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Attack")
	virtual void ServerDie();

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Attack")
	virtual void MulticastDie();

	// 나이아가라 이펙트를 스폰할 때 사용할 나이아가라 액터 클래스 (에디터에서 지정)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	TSubclassOf<AActor> NiagaraActorClass;

	// 나이아가라 이펙트를 스폰하고 1초 뒤 제거하는 함수
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Effects")
	void SpawnAndDestroyNiagaraEffect();

	// 착지 시점을 처리하는 Landed 함수 재정의
	virtual void Landed(const FHitResult& Hit) override;

	UFUNCTION(Server, Reliable, BlueprintCallable)
	virtual void ServerDamageNearByActorsOnLanding();	// 착지 시 데미지를 입힘

	UFUNCTION(Server, Reliable, BlueprintCallable)
	virtual void ServerSetBlackboardKey();

	// 공격 종료 이벤트 델리게이트 (모든 애님 인스턴스가 끝날 때 호출)
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnBossAttackEndDelegate OnBossAttackEnd;

	UPROPERTY(BlueprintAssignable, Category = "BossEvent")
	FOnWakeUp OnWakeUp;

	UPROPERTY(BlueprintAssignable, Category = "Stat")
	FOnTakeDamage OnTakeDamage;

	UFUNCTION(Server, Reliable, Category = "PlayMontage")
	virtual void ServerRoarMontage();

	UFUNCTION(Server, Reliable, Category = "PlayMontage")
	virtual void ServerPlayMontage(UAnimMontage* Montage, const FName& NotifyName);


	UFUNCTION(NetMulticast, Reliable, Category = "PlayMontage")
	virtual void MultiPlayMontage(UAnimMontage* Montage, const FName& NotifyName);


	UFUNCTION(NetMulticast, Reliable)
	void MultiUpdateBossHealth(float NewHealth, float NewMaxHealth);
	void MultiUpdateBossHealth_Implementation(float NewHealth, float NewMaxHealth);

	

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// 인터페이스 함수 호출 후 C++ 구현부 제공
		// C++ 구현부를 제공하려면 _Implementation 접미사가 붙음


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attack")
	void Attack(FName NotifyName);
	virtual void Attack_Implementation(FName NotifyName) override;

	//** Stat **// 

// 현재 체력 저장
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Stat")
	float GetCurrentHealth();
	virtual float GetCurrentHealth_Implementation() override;

	// 최대체력 저장
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Stat")
	float GetMaxHealth();
	virtual float GetMaxHealth_Implementation() override;

	// 데미지를 입을때 호출
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
	void TakeDamage(const float Amount, AActor* DamageActor);
	virtual void TakeDamage_Implementation(float Amount, AActor* DamageActor) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Stat")
	bool IsDead();
	virtual bool IsDead_Implementation() override;



	///* Interface Function End */////
	

	UFUNCTION(Server,Reliable)
	virtual void ServerTakeDamage(const float Amount, AActor* DamageActor);

	UFUNCTION(NetMulticast, Reliable)
	virtual void MultiTakeDamage(const float Amount, AActor* DamageActor);

	virtual UBehaviorTree* GetBehaviorTree();

	// 각 몽타주 재생을 위한 BlueprintCallable 함수들

	UFUNCTION(BlueprintCallable, Category = "Montage")
	virtual void PlayRoarMontage();

	UFUNCTION(BlueprintCallable, Category = "Montage")
	virtual void PlayDeathMontage();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	virtual void Server_ResetBehavior();

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBlackboardComponent* BlackboardComp;

	// 리플리케이션 설정을 위한 오버라이드
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION()
	bool GetIsBossWakeupTriggered();
	UFUNCTION()
	void SetIsBossWakeupTriggered(bool SetBool);

protected:

	virtual void BeginPlay() override;

	

	// **		애님 몽타주		 **//
		// 값은 에디터에서 적용.


									#pragma region Montages
	// 공격1 ...
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* Attack1Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* Attack2Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* Attack3Montage;

	// 공격 4
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* Attack4Montage;

	// 전진공격1
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* FWAttack1Montage;

	// 전진공격2
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* FWAttack2Montage;

	// 점프 덮치기 공격1
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* JumpAttackMontage;

	// 돌격 박치기 공격
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* BiteForwardMontage;

	// 괴성 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* RoarMontage;

	// 죽음 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BehaviorTree", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* BTTree;

	// AICEnemyBase를 저장할 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	AAIC_EnemyBossBase* AICEnemyBase;

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	void OnAttack1MontageEnded(bool bInterrupted);
	void OnAttack2MontageEnded(bool bInterrupted);
	void OnAttack3MontageEnded(bool bInterrupted);
	void OnAttack4MontageEnded(bool bInterrupted);
	void OnFW1AttackMontageEnded(bool bInterrupted);
	void OnFW2AttackMontageEnded(bool bInterrupted);
	void OnJumpAttackMontageEnded(bool bInterrupted);
	void OnBiteForwardMontageEnded(bool bInterrupted);
	void OnRoarMontageEnded(bool bInterrupted);
	void OnDeathMontageEnded(bool bInterrupted);

#pragma endregion	

///////////////////////////

	//**		Stat		**//

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	float MaxHealth = 100;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	float Health = MaxHealth;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	bool bIsDead = false;

	// Enemy Stat

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reset")
	FVector ResetLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Reset")
	AActor* BossResetLocationActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reset")
	AActor* BossResetFocusActor;

private:
	UPROPERTY(Replicated, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool bIsBossWakeupTriggered = false;

	
	

};
