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
class UWidgetComponent;						// UI를 표시하는 컴포넌트
class UUserWidget;							// 사용자 위젯
class UEnemyHealthBar;
template<typename T> class TSubclassOf;		// UUserWidget 클래스를 가리키는 템플릿

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

	virtual UBehaviorTree* GetBehaviorTree();	// AIC_EnemyBase의 Possess에서 참조

	// 몽타주 종료시 호출 
	virtual void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION(BlueprintCallable)
	virtual void Die();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowHealthBarForDuration(float Duration = 1.0f);

	// Die 함수
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Attack")
	virtual void ServerDie();

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Attack")
	virtual void MulticastDie();

	// 공격 몽타주 
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Attack")
	virtual void ServerAttackMontage(const FName& NotifyName);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Attack")
	virtual void MulticastAttackMontage(const FName& NotifyName);

	////
	// 원거리 공격 몽타주 
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Attack")
	virtual void ServerThrowWeaponAtTarget(AActor* InWeapon, AActor* InTarget);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Attack")
	virtual void MulticastThrowWeaponAtTarget(AActor* InWeapon, AActor* InTarget);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Attack")
	virtual void ServerTakeDamage(const float Amount, AActor* DamageActor);




	// 리플리케이션 설정을 위한 오버라이드
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 블루프린트에서 바인딩 할 수 있는 이벤트 디스패쳐. 다른 클래스에서 바인딩하기에 public 선언.

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnAttackEndDelegate OnAttackEnd;				// 공격이 끝났을 시

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnDeathDelegate OnDeathDelegate;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// 인터페이스 함수 호출 후 C++ 구현부 제공
		// C++ 구현부를 제공하려면 _Implementation 접미사가 붙음
	virtual void SetMovementSpeed_Implementation(E_MovementSpeed NewSpeed) override;

	virtual void GetIdealRange_Implementation(float& AttackRadius, float& DefendRadius) const override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attack")
	void Attack(FName NotifyName);
	virtual void Attack_Implementation(FName NotifyName) override;



	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attack")
	void ThrowWeaponAtTarget(AActor* InWeapon, AActor* InTarget);
	virtual void ThrowWeaponAtTarget_Implementation(AActor* InWeapon, AActor* InTarget) override;

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
	virtual void TakeDamage_Implementation(const float Amount, AActor* DamageActor) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Stat")
	bool IsDead();
	virtual bool IsDead_Implementation() override;

	// 타이머를 이용해 Duration 후에 위젯 컴포넌트를 제거하거나 숨깁니다.
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
	TSubclassOf<UUserWidget> HealthBarClass;	// 위젯 클래스를 가리키는 템플릿 포인터

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta=(AllowPrivateAccess = "true"))
	UWidgetComponent* HealthBarWidgetComponent;
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* CharacterWidgetComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	UEnemyHealthBar* MyHealthBar;


	// 공격모션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AttackMontage;

	// C/C++에서 포인터 멤버 변수로 무기 정보를 저장
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equip", meta = (AllowPrivateAccess = "true"))
	bool bIsWieldingWeapon;

	// BP로 만든 무기를 BP 에디터에서 지정.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equip", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> WeaponClass;

	// 실제로 장착된 무기를 가리키는 변수 (BP의 "Set Weapon Actor")
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	AActor* WeaponActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BehaviorTree", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* BehaviorTree;

	// AICEnemyBase를 저장할 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	AAIC_EnemyBase* AICEnemyBase;

	



};
