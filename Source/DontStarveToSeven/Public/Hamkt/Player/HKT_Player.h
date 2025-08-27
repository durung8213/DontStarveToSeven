// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HKT_Player.generated.h"

// 전방선언 : 스프링 암과 카메라 컴포넌트 사용
class USpringArmComponent;
class UCameraComponent;
class UBossStatusWidget;

UCLASS()
class DONTSTARVETOSEVEN_API AHKT_Player : public ACharacter
{
	GENERATED_BODY()

public:
	// 생성자
	AHKT_Player();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// 입력 바인딩을 설정
/*
	입력 바인딩(SetupPlayerInputComponent)

		사용자 입력(키보드 / 마우스, 컨트롤러 등)을 각 함수에 바인딩하여, 입력 이벤트가 발생할 때마다 대응 함수가 호출되도록 합니다.
		이는 C / C++에서 이벤트 리스너나 콜백 함수를 설정하는 것과 비슷한 개념입니다.
*/
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 입력바인딩 (예: BindAxis)이나 내부 로직에서 호출됨 / 
	// 블루프린트 호출 및 리플렉션 시스템 접근 필요 없으면 UFUNCTION() 생략
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);

	UPROPERTY(EditAnywhere)
	int i = 100;

	UFUNCTION(BlueprintCallable)
	void UpdateBossHPBar(float InCurrentHealth, float InMaxHealth); // 예시용 호출 함수

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

	// WBP 보스 HP바 표시
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UBossStatusWidget> BossStatusWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	UBossStatusWidget* BossStatusWidgetInstance;

private:

};
