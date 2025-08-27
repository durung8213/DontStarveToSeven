// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Seojy/BaseCraft.h"
#include "WoodFence.generated.h"

class UStaticMeshComponent;
class UBoxComponent;

/**
 * 
 */
UCLASS()
class DONTSTARVETOSEVEN_API AWoodFence : public ABaseCraft
{
	GENERATED_BODY()
	
public:

	AWoodFence();
	~AWoodFence();

	// �������� ���������� �����ϱ� ���� �Լ�
	UFUNCTION(Server,Reliable)
	void Server_ApplyDamageToEnemy(AActor* EnemyActor);

	// ������ ���� �̺�Ʈ
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	// ������ ���� �̺�Ʈ
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// SetCraftMode �������̵�
	virtual void SetCraftMode(ECraftMode NewMode) override;

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:
	// ���� �ð����� ������ ������
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float Damage;

	// ��å�� ����ƽ �޽�
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* FenceMesh;

	// ��å ������ ���� (�ݸ���)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UBoxComponent* DamageZone;

	// �������� ������ ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float DamageInterval;

	// �� �� ���ͺ��� ������ Ÿ�̸Ӹ� �����ϱ� ���� ��
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<AActor*, FTimerHandle> EnemyDamageTimers;


};
