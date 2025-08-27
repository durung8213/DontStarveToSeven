// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/WidgetComponent.h"
#include "Kimmh/Interface/DTS_BPIInteraction.h"
#include "Leesh/SHTotalStruct.h"
#include "UObject/SoftObjectPtr.h"
#include "NiagaraSystem.h"
#include "Particles/ParticleSystem.h"
#include "FXSystem.h"
#include "Resource.generated.h"

UCLASS()
class DONTSTARVETOSEVEN_API AResource : public AActor, public IDTS_BPIInteraction
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AResource();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 해당 리소스 액터가 오버랩을 실행.
	UFUNCTION()
	void OnOverlap(
		UPrimitiveComponent* HitComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
		);

	UFUNCTION(Server,Reliable)
	void Server_HandleInteraction(FVector ImpactPoint, ABasePlayerCharacter* Player);

	// 클라 -> 서버로 HandleInteraction 실행 요청
	void HandleInteractionInternal(FVector ImpactPoint, ABasePlayerCharacter* Player);

	// 서버에서 히트 처리를 위한 RPC
	UFUNCTION(Server, Reliable)
	void Server_HandleHit();

	// 파티클을 멀티캐스트로 복제
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayParticleEffect(FVector Location);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayDestroyEffect();


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/// <summary>
	/// 인터페이스 함수
	/// 위젯을 반환하는 함수이다
	/// </summary>
	/// <returns>상호작용 위젯의 주소</returns>
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GetWidget")
	UDTS_BaseInteractionUserWidget* GetBaseInteractionWidget();
	virtual UDTS_BaseInteractionUserWidget* GetBaseInteractionWidget_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int GetInteractionHp();
	virtual int GetInteractionHp_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	EInteraction GetInteractionType();
	virtual EInteraction GetInteractionType_Implementation() override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void PlayInitialParticle();

public:
	/// <summary>
	/// 위젯 컴포넌트
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Interaction")
	class UWidgetComponent* InteractionWidget;


	// 시각적 메쉬
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* MeshComp;
	 
	//오버랩을 위한 트리거 영역
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* InteractionTrigger;

	// 현재 플레이어
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	ADTS_GamePlayPlayerController* PlayerPC;


	/* 블루프린트 Edit 변수들 */


	// 채집하는 데 필요한 Hit Count
	UPROPERTY(Replicated,EditAnywhere, BlueprintReadWrite, Category = "Collectible")
	int32 RequiredHits;



	// 파티클 이벤트
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* CollectPar;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	FSoftObjectPath CollectParPath;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* DestroyPar;

	// 인터랙션 Enum 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EInteraction InteractionEnum;

	// 아이템 구조체
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FInventory item;

	// 아이템 구조체에 들어갈 변수들
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemName;

	// 액터 별 파티클 위치 조정
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector ParticleOffset = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector ParticleSize = FVector(1.0f);

};
