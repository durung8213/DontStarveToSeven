// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "DTS_BasePlayerState.generated.h"

/**
 * 
 */



UCLASS()
class DONTSTARVETOSEVEN_API ADTS_BasePlayerState : public APlayerState
{
	GENERATED_BODY()
	
	ADTS_BasePlayerState();

public:
	virtual void BeginPlay() override;

	/// <summary>
	///	������ ���ø�����Ʈ �ϱ� ���� �ʿ��� �Լ�(�����϶��)
	/// </summary>
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// MaxHp�� ��ȯ�ϴ� �Լ�
	float GetMaxHp();

	// CurrentHp�� ��ȯ�ϴ� �Լ�
	float GetCurrentHp();


	// MaxMp�� ��ȯ�ϴ� �Լ�
	float GetMaxMp();

	// CurrentMp�� ��ȯ�ϴ� �Լ�
	float GetCurrentMp();

	float GetAtk();

	float GetDef();

	float GetSpeed();

	float GetCritical();

	float GetCriticalDamage();

	float GetMaxHunger();

	float GetCurrentHunger();

	// �������� MaxHp�� �����ϴ� �Լ�
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerSetMaxHp(float NewMaxHp);

	// MaxHp�� ���ø�����Ʈ �Լ�
	UFUNCTION()
	void OnRep_ChangeMaxHp();

	// �������� CurrentHp�� �����ϴ� �Լ�
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerSetCurrentHp(float NewCurrentHp);

	// Current Hp�� ���ø�����Ʈ �Լ�
	UFUNCTION()
	void OnRep_ChangeCurrentHp();

	// �������� MaxMp�� �ٲ��ִ� �Լ�
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerSetMaxMp(float NewMaxMp);

	// MaxMp�� ���ø�����Ʈ �ϴ� �Լ�
	UFUNCTION()
	void OnRep_ChangeMaxMp();

	// �������� CurrentMp�� �ٲ��ִ� �Լ�
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerSetCurrnetMp(float NewCurrentMp);

	// CurrentMp�� ���ø�����Ʈ �Լ�
	UFUNCTION()
	void OnRep_ChangeCurrentMp();

	// �������� Atk�� �ٲ��ִ� �Լ�
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerSetAtk(float NewAtk);

	// �������� Def�� �ٲ��ִ� �Լ�
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerSetDef(float NewDef);

	// �������� Speed�� �ٲ��ִ� �Լ�
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerSetSpeed(float NewSpeed);

	// �������� Critical�� �ٲ��ִ� �Լ�
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerSetCritical(float NewCritical);

	// �������� CriticalDamage�� �ٲ��ִ� �Լ�
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerSetCriticalDamage(float NewCriticalDamage);

	// �������� MaxHunger�� �ٲ��ִ� �Լ�
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerSetMaxHunger(float NewMaxHunger);

	// CurrentMp�� ���ø�����Ʈ �Լ�
	UFUNCTION()
	void OnRep_ChangeMaxHunger();

	// �������� CurrentHunger�� �ٲ��ִ� �Լ�
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerSetCurrentHunger(float NewCurrentHunger);

	// CurrentMp�� ���ø�����Ʈ �Լ�
	UFUNCTION()
	void OnRep_ChangeCurrentHunger();

	// Hit�� ����Ǵ� �Լ� TakeDamage(���ݸ� ����)
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerTakeDamage(float Damage, AActor* DamageActor);

	struct FCharacterStat GetAllPlayerStat();

	void SetAllPlayerStat(const struct FCharacterStat& InStat );

	class UDTS_UserStateUserWidget* GetUserStateUserWidget();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_ChangeMaxHp, Category = "PlayerStance")
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_ChangeCurrentHp, Category = "PlayerStance")
	float CurrentHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_ChangeMaxMp, Category = "PlayerStance")
	float MaxMp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_ChangeCurrentMp, Category = "PlayerStance")
	float CurrentMp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "PlayerStance")
	float Atk;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "PlayerStance")
	float Def;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "PlayerStance")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "PlayerStance")
	float Critical;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "PlayerStance")
	float CriticalDamage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_ChangeMaxHunger, Category = "PlayerStance")
	float MaxHunger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_ChangeCurrentHunger, Category = "PlayerStance")
	float CurrentHunger;
};
