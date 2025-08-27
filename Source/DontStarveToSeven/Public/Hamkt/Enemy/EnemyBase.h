// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "Hamkt/Enemy/AI/BPI_EnemyAI.h"
#include "EnemyBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackEndDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathDelegate);

class AAIC_EnemyBase;
class UWidgetComponent;						// UI�� ǥ���ϴ� ������Ʈ
class UUserWidget;							// ����� ����
class UEnemyHealthBar;
template<typename T> class TSubclassOf;		// UUserWidget Ŭ������ ����Ű�� ���ø�

UCLASS()
class DONTSTARVETOSEVEN_API AEnemyBase : public ACharacter, public IBPI_EnemyAI
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual UBehaviorTree* GetBehaviorTree();	// AIC_EnemyBase�� Possess���� ����

	// ��Ÿ�� ����� ȣ�� 
	virtual void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION(BlueprintCallable)
	virtual void Die();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowHealthBarForDuration(float Duration = 1.0f);

	// Die �Լ�
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Attack")
	virtual void ServerDie();

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Attack")
	virtual void MulticastDie();

	// ���� ��Ÿ�� 
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Attack")
	virtual void ServerAttackMontage(const FName& NotifyName);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Attack")
	virtual void MulticastAttackMontage(const FName& NotifyName);

	////
	// ���Ÿ� ���� ��Ÿ�� 
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Attack")
	virtual void ServerThrowWeaponAtTarget(AActor* InWeapon, AActor* InTarget);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Attack")
	virtual void MulticastThrowWeaponAtTarget(AActor* InWeapon, AActor* InTarget);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Attack")
	virtual void ServerTakeDamage(const float Amount, AActor* DamageActor);




	// ���ø����̼� ������ ���� �������̵�
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// �������Ʈ���� ���ε� �� �� �ִ� �̺�Ʈ ������. �ٸ� Ŭ�������� ���ε��ϱ⿡ public ����.

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnAttackEndDelegate OnAttackEnd;				// ������ ������ ��

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnDeathDelegate OnDeathDelegate;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// �������̽� �Լ� ȣ�� �� C++ ������ ����
		// C++ �����θ� �����Ϸ��� _Implementation ���̻簡 ����
	virtual void SetMovementSpeed_Implementation(E_MovementSpeed NewSpeed) override;

	virtual void GetIdealRange_Implementation(float& AttackRadius, float& DefendRadius) const override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attack")
	void Attack(FName NotifyName);
	virtual void Attack_Implementation(FName NotifyName) override;



	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attack")
	void ThrowWeaponAtTarget(AActor* InWeapon, AActor* InTarget);
	virtual void ThrowWeaponAtTarget_Implementation(AActor* InWeapon, AActor* InTarget) override;

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
	virtual void TakeDamage_Implementation(const float Amount, AActor* DamageActor) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Stat")
	bool IsDead();
	virtual bool IsDead_Implementation() override;

	// Ÿ�̸Ӹ� �̿��� Duration �Ŀ� ���� ������Ʈ�� �����ϰų� ����ϴ�.
	FTimerHandle HpBarTimerHandle;

				///* Interface Function End */////

protected:	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


							//**		Stat		**//
	// Enemy Stat
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MaxHealth = 100;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float Health = 100;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Stat")
	bool bIsDead = false;


							//**	Compoenent		**//

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> HealthBarClass;	// ���� Ŭ������ ����Ű�� ���ø� ������

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta=(AllowPrivateAccess = "true"))
	UWidgetComponent* HealthBarWidgetComponent;
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* CharacterWidgetComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	UEnemyHealthBar* MyHealthBar;


	// ���ݸ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AttackMontage;

	// C/C++���� ������ ��� ������ ���� ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equip", meta = (AllowPrivateAccess = "true"))
	bool bIsWieldingWeapon;

	// BP�� ���� ���⸦ BP �����Ϳ��� ����.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equip", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> WeaponClass;

	// ������ ������ ���⸦ ����Ű�� ���� (BP�� "Set Weapon Actor")
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	AActor* WeaponActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BehaviorTree", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* BehaviorTree;

	// AICEnemyBase�� ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	AAIC_EnemyBase* AICEnemyBase;

	



};
