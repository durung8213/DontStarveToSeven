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

	// UAIPerceptionStimuliSourceComponent�� �̸� ���� �����մϴ�.
	class UAIPerceptionStimuliSourceComponent;

public:
	ABaseCraft();
	virtual void Tick(float DeltaTime) override;

	//���Ͱ� ������ �� ������ ��Ÿ����.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// ���� ��忡 ���� ���� ���ϱ�
	UFUNCTION(BlueprintCallable, Category = "Ghost")
	void SetPreviewColor(bool bIsCanPlace);

	// ũ����Ʈ ��� ����
	UFUNCTION(BlueprintCallable, Category = "CraftMode")
	virtual void SetCraftMode(ECraftMode NewMode);

	//HP ȹ�� ( ���� )
	UFUNCTION(BlueprintPure, Category = "CraftHP")
	float GetCraftHP() const;

	//HP ���� ( ���� ���� / ���� )
	UFUNCTION(Server, Reliable)
	void ApplyDamage(bool IsPlus, float amount);

	// ũ����Ʈ ���� �Լ�
	UFUNCTION()
	void BrokenCraft();

	// ������ ����
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

	// ������ ���� �޾��� ��, �Լ�
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Stat")
	void TakeDamage(const float Amount, AActor* DamageActor);
	//virtual void TakeDamage_Implementation(float Amount, AActor* DamageActor) override;


	/// <summary>
	/// �������̽� �Լ�
	/// ������ ��ȯ�ϴ� �Լ��̴�
	/// </summary>
	/// <returns>��ȣ�ۿ� ������ �ּ�</returns>
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GetWidget")
	UDTS_BaseInteractionUserWidget* GetBaseInteractionWidget();
	virtual UDTS_BaseInteractionUserWidget* GetBaseInteractionWidget_Implementation() override;

	// ���ͷ��� Ÿ�� ��ȯ
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	EInteraction GetInteractionType();
	virtual EInteraction GetInteractionType_Implementation() override;

	// HP �� �ٲ� ������ UI Ȥ�� ��ƼŬ ǥ��.
	UFUNCTION()
	void OnRep_CraftHP();

	// ���� ���ϴ� ��ƼŬ ����
	UFUNCTION(NetMulticast, Reliable)
	void Multi_PlayHitEffect(const FVector& SpawnLocation);

	UFUNCTION(NetMulticast, Reliable)
	void Multi_PlayDestroyEffect();




protected:
	virtual void BeginPlay() override;

public:

	// ũ������(��Ʈ) / ���� ��� (�÷��̾� ��ȣ�ۿ�)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "CraftMode")
	ECraftMode CurrentMode = ECraftMode::Ghost;;

	//��Ʈ ���׸���
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Material")
	UMaterialInterface* GhostMaterial;

	//���� ���׸���
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Material")
	UMaterialInterface* ActualMaterial;

	// ���� Mesh Component
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	TArray<UStaticMeshComponent*> ActorMeshComponents;

	// �� �ǹ� ������ ID
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Craft")
	FGuid CraftID;

	// ũ����Ʈ ü��
	UPROPERTY(ReplicatedUsing = OnRep_CraftHP)
	float CraftHP = 100.0f;

	// ���׸��� �ε���
	int32 CurrentMaterialIndex = 0;

	// �ݸ��� ������
	float CollisionSize = 0.0f;


	// �� �ǹ��� ������ (�� ũ�� ���� ��)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "CraftMode")
	FBuildingData BuildingData;

	//���� ���׸��� �迭
	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> DynamicMatInstances;

	UPROPERTY()
	FString CraftName;

	// ���� ��ġ ���� ����
	bool bIsCanBuild = false;

	// ���� ��ƼŬ
	UPROPERTY(EditAnywhere)
	UParticleSystem* DestroyEffect;

	// Hit ��ƼŬ
	UPROPERTY(EditAnywhere)
	UParticleSystem* HitEffect;

	/// <summary>
	/// ���� ������Ʈ
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	class UWidgetComponent* InteractionWidget;

	UPROPERTY()
	EInteraction InteractionEnum;

protected:
	// ��忡 ���� �ʱ�ȭ �Լ�
	virtual void ApplyBuildingMode();

	// ���� �����Ϳ� ���õ� ������ �ʱ�ȭ.
	void SetBuildingData(int32 Width, int32 Height, ECraftType CraftType);

private:

};
