// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Hamkt/Enemy/AI/BPI_EnemyAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyBossBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWakeUp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTakeDamage, float, InCurrentHealth, float, InMaxHealth);
// ��������Ʈ ����: �� ���� �Ű������� ���� (����� ��Ÿ�ֿ� �ߴ� ����)
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

	// ���̾ư��� ����Ʈ�� ������ �� ����� ���̾ư��� ���� Ŭ���� (�����Ϳ��� ����)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	TSubclassOf<AActor> NiagaraActorClass;

	// ���̾ư��� ����Ʈ�� �����ϰ� 1�� �� �����ϴ� �Լ�
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Effects")
	void SpawnAndDestroyNiagaraEffect();

	// ���� ������ ó���ϴ� Landed �Լ� ������
	virtual void Landed(const FHitResult& Hit) override;

	UFUNCTION(Server, Reliable, BlueprintCallable)
	virtual void ServerDamageNearByActorsOnLanding();	// ���� �� �������� ����

	UFUNCTION(Server, Reliable, BlueprintCallable)
	virtual void ServerSetBlackboardKey();

	// ���� ���� �̺�Ʈ ��������Ʈ (��� �ִ� �ν��Ͻ��� ���� �� ȣ��)
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

	// �������̽� �Լ� ȣ�� �� C++ ������ ����
		// C++ �����θ� �����Ϸ��� _Implementation ���̻簡 ����


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attack")
	void Attack(FName NotifyName);
	virtual void Attack_Implementation(FName NotifyName) override;

	//** Stat **// 

// ���� ü�� ����
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Stat")
	float GetCurrentHealth();
	virtual float GetCurrentHealth_Implementation() override;

	// �ִ�ü�� ����
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Stat")
	float GetMaxHealth();
	virtual float GetMaxHealth_Implementation() override;

	// �������� ������ ȣ��
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

	// �� ��Ÿ�� ����� ���� BlueprintCallable �Լ���

	UFUNCTION(BlueprintCallable, Category = "Montage")
	virtual void PlayRoarMontage();

	UFUNCTION(BlueprintCallable, Category = "Montage")
	virtual void PlayDeathMontage();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	virtual void Server_ResetBehavior();

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBlackboardComponent* BlackboardComp;

	// ���ø����̼� ������ ���� �������̵�
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION()
	bool GetIsBossWakeupTriggered();
	UFUNCTION()
	void SetIsBossWakeupTriggered(bool SetBool);

protected:

	virtual void BeginPlay() override;

	

	// **		�ִ� ��Ÿ��		 **//
		// ���� �����Ϳ��� ����.


									#pragma region Montages
	// ����1 ...
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* Attack1Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* Attack2Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* Attack3Montage;

	// ���� 4
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* Attack4Montage;

	// ��������1
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* FWAttack1Montage;

	// ��������2
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* FWAttack2Montage;

	// ���� ��ġ�� ����1
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* JumpAttackMontage;

	// ���� ��ġ�� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* BiteForwardMontage;

	// ���� �ִϸ��̼�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* RoarMontage;

	// ���� �ִϸ��̼�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BehaviorTree", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* BTTree;

	// AICEnemyBase�� ������ ����
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
