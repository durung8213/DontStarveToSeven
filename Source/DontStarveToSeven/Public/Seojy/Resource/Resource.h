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

	// �ش� ���ҽ� ���Ͱ� �������� ����.
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

	// Ŭ�� -> ������ HandleInteraction ���� ��û
	void HandleInteractionInternal(FVector ImpactPoint, ABasePlayerCharacter* Player);

	// �������� ��Ʈ ó���� ���� RPC
	UFUNCTION(Server, Reliable)
	void Server_HandleHit();

	// ��ƼŬ�� ��Ƽĳ��Ʈ�� ����
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayParticleEffect(FVector Location);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayDestroyEffect();


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/// <summary>
	/// �������̽� �Լ�
	/// ������ ��ȯ�ϴ� �Լ��̴�
	/// </summary>
	/// <returns>��ȣ�ۿ� ������ �ּ�</returns>
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
	/// ���� ������Ʈ
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Interaction")
	class UWidgetComponent* InteractionWidget;


	// �ð��� �޽�
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* MeshComp;
	 
	//�������� ���� Ʈ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* InteractionTrigger;

	// ���� �÷��̾�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	ADTS_GamePlayPlayerController* PlayerPC;


	/* �������Ʈ Edit ������ */


	// ä���ϴ� �� �ʿ��� Hit Count
	UPROPERTY(Replicated,EditAnywhere, BlueprintReadWrite, Category = "Collectible")
	int32 RequiredHits;



	// ��ƼŬ �̺�Ʈ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* CollectPar;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	FSoftObjectPath CollectParPath;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* DestroyPar;

	// ���ͷ��� Enum ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EInteraction InteractionEnum;

	// ������ ����ü
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FInventory item;

	// ������ ����ü�� �� ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemName;

	// ���� �� ��ƼŬ ��ġ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector ParticleOffset = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector ParticleSize = FVector(1.0f);

};
