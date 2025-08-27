// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HKT_Player.generated.h"

// ���漱�� : ������ �ϰ� ī�޶� ������Ʈ ���
class USpringArmComponent;
class UCameraComponent;
class UBossStatusWidget;

UCLASS()
class DONTSTARVETOSEVEN_API AHKT_Player : public ACharacter
{
	GENERATED_BODY()

public:
	// ������
	AHKT_Player();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// �Է� ���ε��� ����
/*
	�Է� ���ε�(SetupPlayerInputComponent)

		����� �Է�(Ű���� / ���콺, ��Ʈ�ѷ� ��)�� �� �Լ��� ���ε��Ͽ�, �Է� �̺�Ʈ�� �߻��� ������ ���� �Լ��� ȣ��ǵ��� �մϴ�.
		�̴� C / C++���� �̺�Ʈ �����ʳ� �ݹ� �Լ��� �����ϴ� �Ͱ� ����� �����Դϴ�.
*/
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// �Է¹��ε� (��: BindAxis)�̳� ���� �������� ȣ��� / 
	// �������Ʈ ȣ�� �� ���÷��� �ý��� ���� �ʿ� ������ UFUNCTION() ����
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);

	UPROPERTY(EditAnywhere)
	int i = 100;

	UFUNCTION(BlueprintCallable)
	void UpdateBossHPBar(float InCurrentHealth, float InMaxHealth); // ���ÿ� ȣ�� �Լ�

	UFUNCTION(Server, Reliable)
	void ServerBossHPBar(float InCurrentHealth, float InMaxHealth);

	UFUNCTION(NetMulticast, Reliable)
	void MultiBossHPBar(float InCurrentHealth, float InMaxHealth);

	UFUNCTION(Server, Reliable)
	void ServerBossWidgetUpdate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* FollowCamera;

	// WBP ���� HP�� ǥ��
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UBossStatusWidget> BossStatusWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	UBossStatusWidget* BossStatusWidgetInstance;

private:

};
