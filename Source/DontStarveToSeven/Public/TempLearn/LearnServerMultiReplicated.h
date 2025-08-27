// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LearnServerMultiReplicated.generated.h"

UCLASS()
class DONTSTARVETOSEVEN_API ALearnServerMultiReplicated : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALearnServerMultiReplicated();

	/// <summary>
	///	변수를 리플리케이트 하기 위해 필요한 함수
	/// 이 함수에 변수를 등록해야 리프리케이트가 정상적으로 작동한다
	/// </summary>
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 리플리케이트 노티파이 함수 선언, 변수의 리플리케이트가 일어날 때 클라이언트에서 실행된다
	UFUNCTION()
	void OnRep_ChangeCurrentHp();

	UFUNCTION(Server, Reliable)	// Server : 서버에서 실행, Reliable : 신뢰성)
	void ServerFunc();

	UFUNCTION(NetMulticast, Reliable)	// NetMulticast : 멀티케스트로 실행, Reliable : 신뢰성)
	void MultiFunc();

	UFUNCTION(Client, Reliable)	// Client : 클라이언트에서 실행, Reliable : 신뢰성)
	void ClientFunc();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ------------------------- 변수 리플리케이트 -----------------------------
	
	// 리플리케이트 UPROPERTY(리플리케이트)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "PlayerStance")
	float MaxHp;

	// 리플리케이트 노티파이 UPROPERTY(ReplicatedUsing = 리플리케이트 노티파이 함수명)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_ChangeCurrentHp, Category = "PlayerStance")
	float CurrentHp;

};
