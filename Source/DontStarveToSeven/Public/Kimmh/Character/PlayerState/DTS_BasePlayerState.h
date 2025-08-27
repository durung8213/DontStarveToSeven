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
	///	변수를 리플리케이트 하기 위해 필요한 함수(참고하라고)
	/// </summary>
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// MaxHp를 반환하는 함수
	float GetMaxHp();

	// CurrentHp를 반환하는 함수
	float GetCurrentHp();


	// MaxMp를 반환하는 함수
	float GetMaxMp();

	// CurrentMp를 반환하는 함수
	float GetCurrentMp();

	float GetAtk();

	float GetDef();

	float GetSpeed();

	float GetCritical();

	float GetCriticalDamage();

	float GetMaxHunger();

	float GetCurrentHunger();

	// 서버에서 MaxHp를 변경하는 함수
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerSetMaxHp(float NewMaxHp);

	// MaxHp의 리플리케이트 함수
	UFUNCTION()
	void OnRep_ChangeMaxHp();

	// 서버에서 CurrentHp를 변경하는 함수
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerSetCurrentHp(float NewCurrentHp);

	// Current Hp의 리플리케이트 함수
	UFUNCTION()
	void OnRep_ChangeCurrentHp();

	// 서버에서 MaxMp를 바꿔주는 함수
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerSetMaxMp(float NewMaxMp);

	// MaxMp를 리플리케이트 하는 함수
	UFUNCTION()
	void OnRep_ChangeMaxMp();

	// 서버에서 CurrentMp를 바꿔주는 함수
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerSetCurrnetMp(float NewCurrentMp);

	// CurrentMp의 리플리케이트 함수
	UFUNCTION()
	void OnRep_ChangeCurrentMp();

	// 서버에서 Atk를 바꿔주는 함수
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerSetAtk(float NewAtk);

	// 서버에서 Def를 바꿔주는 함수
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerSetDef(float NewDef);

	// 서버에서 Speed를 바꿔주는 함수
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerSetSpeed(float NewSpeed);

	// 서버에서 Critical를 바꿔주는 함수
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerSetCritical(float NewCritical);

	// 서버에서 CriticalDamage를 바꿔주는 함수
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerSetCriticalDamage(float NewCriticalDamage);

	// 서버에서 MaxHunger를 바꿔주는 함수
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerSetMaxHunger(float NewMaxHunger);

	// CurrentMp의 리플리케이트 함수
	UFUNCTION()
	void OnRep_ChangeMaxHunger();

	// 서버에서 CurrentHunger를 바꿔주는 함수
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerSetCurrentHunger(float NewCurrentHunger);

	// CurrentMp의 리플리케이트 함수
	UFUNCTION()
	void OnRep_ChangeCurrentHunger();

	// Hit시 실행되는 함수 TakeDamage(스텟만 변경)
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
