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

	// 데미지를 지속적으로 적용하기 위한 함수
	UFUNCTION(Server,Reliable)
	void Server_ApplyDamageToEnemy(AActor* EnemyActor);

	// 오버랩 시작 이벤트
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	// 오버랩 종료 이벤트
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// SetCraftMode 오버라이드
	virtual void SetCraftMode(ECraftMode NewMode) override;

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:
	// 일정 시간마다 적용할 데미지
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float Damage;

	// 목책의 스태틱 메시
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* FenceMesh;

	// 목책 데미지 적용 (콜리전)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UBoxComponent* DamageZone;

	// 데미지를 적용할 간격
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float DamageInterval;

	// 각 적 액터별로 데미지 타이머를 관리하기 위한 맵
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<AActor*, FTimerHandle> EnemyDamageTimers;


};
