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
	
	// �� ���� ���ΰ� ����� �� ����Ǵ� �Լ�
	UFUNCTION()
	void OnRep_FireOn();

	// �������� ���� Ű�� �Լ�
	UFUNCTION(Server, Reliable)
	void ServerTurnOnFire();

	
	// Ʈ���� ���� ����
	UFUNCTION()
	void OnTriggerBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);


	// Ʈ���� ���� ����
	UFUNCTION()
	void OnTriggerEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// �������̽� �Լ�, ��ȣ�ۿ� Ű�� ���� �� �����
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Fire")
	void Interaction(ADTS_GamePlayPlayerController* InteractionPlayerController, ABasePlayerCharacter* InteractionPlayerCharacter);
	virtual void Interaction_Implementation(ADTS_GamePlayPlayerController* InteractionPlayerController, ABasePlayerCharacter* InteractionPlayerCharacter) override;

	// �������� �� ��ġ
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerPlaceCrockPot();



private:
	UFUNCTION()
	void CheckInventory();

public:

	// ��ں� ����ƽ �޽�
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = "Mesh")
	UStaticMeshComponent* FireMesh;


	// ���� �ִ��� ������ Ȯ���ϴ� �� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_FireOn)
	bool bIsFireOn = false;

	// �÷��̾ ������ �ִ� �� Ȯ���ϴ� Ʈ���� ���Ǿ�.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CampFire")
	class USphereComponent* Trigger;

	// ���� �÷��̾ ���� ������ �ִ��� �ƴ��� Ȯ���ϴ� ����
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	bool bIsHavePot = false;

	// ��ں� ��ƼŬ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CampFire")
	class UParticleSystemComponent* FireParticle;

	// ���� Ʈ���� �ȿ� �÷��̾ �ִ��� ������ Ȯ��.
	UPROPERTY(Replicated, VisibleAnywhere)
	bool bPlayerInRange;

	// ���� ������ ������ �ִ��� �ƴ���
	UPROPERTY(Replicated, VisibleAnywhere)
	bool bIsHaveWood = false;

	// �丮�� Ŭ����
	UPROPERTY(EditDefaultsOnly, Category = "BonFire")
	TSubclassOf<ACrockPot> CrockPotClass;

protected:
	virtual void Tick(float DeltaTime) override;



private:


	// Ʈ���� �ȿ� ���� �� ������ Ÿ�̸�
	FTimerHandle TimerHandle_CheckInventory;

	//�������� ĳ������ ��Ʈ�ѷ�
	ADTS_GamePlayPlayerController* PlayerPC = nullptr;


public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
