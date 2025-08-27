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
	///	������ ���ø�����Ʈ �ϱ� ���� �ʿ��� �Լ�
	/// �� �Լ��� ������ ����ؾ� ����������Ʈ�� ���������� �۵��Ѵ�
	/// </summary>
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// ���ø�����Ʈ ��Ƽ���� �Լ� ����, ������ ���ø�����Ʈ�� �Ͼ �� Ŭ���̾�Ʈ���� ����ȴ�
	UFUNCTION()
	void OnRep_ChangeCurrentHp();

	UFUNCTION(Server, Reliable)	// Server : �������� ����, Reliable : �ŷڼ�)
	void ServerFunc();

	UFUNCTION(NetMulticast, Reliable)	// NetMulticast : ��Ƽ�ɽ�Ʈ�� ����, Reliable : �ŷڼ�)
	void MultiFunc();

	UFUNCTION(Client, Reliable)	// Client : Ŭ���̾�Ʈ���� ����, Reliable : �ŷڼ�)
	void ClientFunc();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ------------------------- ���� ���ø�����Ʈ -----------------------------
	
	// ���ø�����Ʈ UPROPERTY(���ø�����Ʈ)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "PlayerStance")
	float MaxHp;

	// ���ø�����Ʈ ��Ƽ���� UPROPERTY(ReplicatedUsing = ���ø�����Ʈ ��Ƽ���� �Լ���)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_ChangeCurrentHp, Category = "PlayerStance")
	float CurrentHp;

};
