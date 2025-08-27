// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Seojy/BaseCraft.h"
#include "Seojy/Craft/CrockPot.h"
#include "GameFramework/Character.h"
#include "Kimmh/Character/Controller/DTS_GamePlayPlayerController.h"

#include "BonFire.generated.h"

/**
 * 
 */
UCLASS()
class DONTSTARVETOSEVEN_API ABonFire : public ABaseCraft
{
	GENERATED_BODY()
public:
	ABonFire();
	~ABonFire();

	virtual void SetCraftMode(ECraftMode NewMode) override;

protected:
	virtual void BeginPlay() override;
	
	// 불 켜짐 여부가 변경될 때 실행되는 함수
	UFUNCTION()
	void OnRep_FireOn();

	// 서버에서 불을 키는 함수
	UFUNCTION(Server, Reliable)
	void ServerTurnOnFire();

	
	// 트리거 영역 입장
	UFUNCTION()
	void OnTriggerBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);


	// 트리거 영역 나감
	UFUNCTION()
	void OnTriggerEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// 인터페이스 함수, 상호작용 키를 누를 시 실행됨
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Fire")
	void Interaction(ADTS_GamePlayPlayerController* InteractionPlayerController, ABasePlayerCharacter* InteractionPlayerCharacter);
	virtual void Interaction_Implementation(ADTS_GamePlayPlayerController* InteractionPlayerController, ABasePlayerCharacter* InteractionPlayerCharacter) override;

	// 서버에서 솥 설치
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerPlaceCrockPot();



private:
	UFUNCTION()
	void CheckInventory();

public:

	// 모닥불 스태틱 메쉬
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = "Mesh")
	UStaticMeshComponent* FireMesh;


	// 불이 있는지 없는지 확인하는 불 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_FireOn)
	bool bIsFireOn = false;

	// 플레이어가 가까이 있는 걸 확인하는 트리거 스피어.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CampFire")
	class USphereComponent* Trigger;

	// 현재 플레이어가 솥을 가지고 있는지 아닌지 확인하는 변수
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	bool bIsHavePot = false;

	// 모닥불 파티클
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CampFire")
	class UParticleSystemComponent* FireParticle;

	// 현재 트리거 안에 플레이어가 있는지 없는지 확인.
	UPROPERTY(Replicated, VisibleAnywhere)
	bool bPlayerInRange;

	// 현재 나무를 가지고 있는지 아닌지
	UPROPERTY(Replicated, VisibleAnywhere)
	bool bIsHaveWood = false;

	// 요리솥 클래스
	UPROPERTY(EditDefaultsOnly, Category = "BonFire")
	TSubclassOf<ACrockPot> CrockPotClass;

protected:
	virtual void Tick(float DeltaTime) override;



private:


	// 트리거 안에 있을 때 진행할 타이머
	FTimerHandle TimerHandle_CheckInventory;

	//오버랩된 캐릭터의 컨트롤러
	ADTS_GamePlayPlayerController* PlayerPC = nullptr;


public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
