// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Seojy/Struct/BuildingData.h"
#include "Kimmh/Interface/DTS_BPIInteraction.h"
#include "Components/WidgetComponent.h"
#include "BaseCraft.generated.h"

UCLASS(Blueprintable)
class DONTSTARVETOSEVEN_API ABaseCraft : public AActor, public IDTS_BPIInteraction
{
	GENERATED_BODY()

	// UAIPerceptionStimuliSourceComponent를 미리 전방 선언합니다.
	class UAIPerceptionStimuliSourceComponent;

public:
	ABaseCraft();
	virtual void Tick(float DeltaTime) override;

	//액터가 복제될 수 있음을 나타낸다.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 현재 모드에 따른 색깔 정하기
	UFUNCTION(BlueprintCallable, Category = "Ghost")
	void SetPreviewColor(bool bIsCanPlace);

	// 크래프트 모드 설정
	UFUNCTION(BlueprintCallable, Category = "CraftMode")
	virtual void SetCraftMode(ECraftMode NewMode);

	//HP 획득 ( 수리 )
	UFUNCTION(BlueprintPure, Category = "CraftHP")
	float GetCraftHP() const;

	//HP 증감 ( 공격 받음 / 수리 )
	UFUNCTION(Server, Reliable)
	void ApplyDamage(bool IsPlus, float amount);

	// 크래프트 삭제 함수
	UFUNCTION()
	void BrokenCraft();

	// 오버랩 실행
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

	// 적에게 공격 받았을 때, 함수
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Stat")
	void TakeDamage(const float Amount, AActor* DamageActor);
	//virtual void TakeDamage_Implementation(float Amount, AActor* DamageActor) override;


	/// <summary>
	/// 인터페이스 함수
	/// 위젯을 반환하는 함수이다
	/// </summary>
	/// <returns>상호작용 위젯의 주소</returns>
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GetWidget")
	UDTS_BaseInteractionUserWidget* GetBaseInteractionWidget();
	virtual UDTS_BaseInteractionUserWidget* GetBaseInteractionWidget_Implementation() override;

	// 인터랙션 타입 반환
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	EInteraction GetInteractionType();
	virtual EInteraction GetInteractionType_Implementation() override;

	// HP 가 바뀔 때마다 UI 혹은 파티클 표시.
	UFUNCTION()
	void OnRep_CraftHP();

	// 공격 당하는 파티클 생성
	UFUNCTION(NetMulticast, Reliable)
	void Multi_PlayHitEffect(const FVector& SpawnLocation);

	UFUNCTION(NetMulticast, Reliable)
	void Multi_PlayDestroyEffect();




protected:
	virtual void BeginPlay() override;

public:

	// 크래프팅(고스트) / 원본 모드 (플레이어 상호작용)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "CraftMode")
	ECraftMode CurrentMode = ECraftMode::Ghost;;

	//고스트 머테리얼
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Material")
	UMaterialInterface* GhostMaterial;

	//원본 머테리얼
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Material")
	UMaterialInterface* ActualMaterial;

	// 액터 Mesh Component
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	TArray<UStaticMeshComponent*> ActorMeshComponents;

	// 각 건물 액터의 ID
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Craft")
	FGuid CraftID;

	// 크래프트 체력
	UPROPERTY(ReplicatedUsing = OnRep_CraftHP)
	float CraftHP = 100.0f;

	// 머테리얼 인덱스
	int32 CurrentMaterialIndex = 0;

	// 콜리전 사이즈
	float CollisionSize = 0.0f;


	// 각 건물의 데이터 (셀 크기 차지 수)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "CraftMode")
	FBuildingData BuildingData;

	//동적 머테리얼 배열
	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> DynamicMatInstances;

	UPROPERTY()
	FString CraftName;

	// 현재 설치 가능 상태
	bool bIsCanBuild = false;

	// 삭제 파티클
	UPROPERTY(EditAnywhere)
	UParticleSystem* DestroyEffect;

	// Hit 파티클
	UPROPERTY(EditAnywhere)
	UParticleSystem* HitEffect;

	/// <summary>
	/// 위젯 컴포넌트
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	class UWidgetComponent* InteractionWidget;

	UPROPERTY()
	EInteraction InteractionEnum;

protected:
	// 모드에 따른 초기화 함수
	virtual void ApplyBuildingMode();

	// 빌딩 데이터에 관련된 변수를 초기화.
	void SetBuildingData(int32 Width, int32 Height, ECraftType CraftType);

private:

};
