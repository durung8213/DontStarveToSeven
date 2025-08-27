// Fill out your copyright notice in the Description page of Project Settings.


#include "Hamkt/Player/HKT_Player.h"
#include "GameFramework/Controller.h"			// 컨트롤러 관련 함수 사용
#include "GameFramework/SpringArmComponent.h"	// 스프링암 컴포넌트 사용
#include "GameFramework/CharacterMovementComponent.h"	// GetCharacterMovement() 반환타입을 사용하기 위해 필수
#include "Components/InputComponent.h"			// 입력 처리 관련 헤더
#include "Engine/World.h"						// GetWorld() 함수 사용
#include "Camera/CameraComponent.h"				// 카메라 컴포넌트 사용
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Hamkt/Widget/BossStatusWidget.h"
#include "Hamkt/Enemy/EnemyBoss/EnemyBossBase.h"

// Sets default values
AHKT_Player::AHKT_Player()
{
 	// 매 프레임 틱함수 호출 허용
	PrimaryActorTick.bCanEverTick = true;

	// 캐릭터가 컨트롤러의 회전값을 직접 사용
	bUseControllerRotationYaw = false;		// Yaw (좌우 회전) x
	bUseControllerRotationPitch = false;	// Pitch(상하 회전)는 캐릭터에 적용 x
	bUseControllerRotationRoll = false;		// Roll(좌우 기울기)는 적용 x

	// Mesh() Location 조정
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -96.f));

	// Mesh() Rotation 조정
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	// 이동방향으로 캐릭터 정면이 회전
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // 캐릭터 회전비율
	GetCharacterMovement()->MaxWalkSpeed = 800.0f;

	// 스프링암 컴포넌트 생성 및 초기화
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);		// 스프링암을 루트 컴포넌트에 부착
	CameraBoom->TargetArmLength = 300.f;			// 카메라와 캐릭터 사이 기본 거리 설정 (300 unit)
	CameraBoom->bUsePawnControlRotation = true;		// 컨트롤러의 회전값을 스프링암에 반영

	// 카메라 컴포넌트 생성 및 초기화
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));	// 생성
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);		// 카메라를 스프링암에 부착.
	FollowCamera->bUsePawnControlRotation = false;									// 카메라 자체 회전 금지 (스프링암이 돌아가게 설정)

	AEnemyBossBase* Boss = Cast<AEnemyBossBase>(UGameplayStatics::GetActorOfClass(GetWorld(), AEnemyBossBase::StaticClass()));
	if (Boss)
	{
		Boss->OnTakeDamage.AddDynamic(this, &AHKT_Player::UpdateBossHPBar);
	}
	
}

void AHKT_Player::UpdateBossHPBar(float InCurrentHealth, float InMaxHealth)
{
	ServerBossHPBar(InCurrentHealth, InMaxHealth);
}

void AHKT_Player::ServerBossHPBar_Implementation(float InCurrentHealth, float InMaxHealth)
{
	MultiBossHPBar(InCurrentHealth, InMaxHealth);
}

void AHKT_Player::MultiBossHPBar_Implementation(float InCurrentHealth, float InMaxHealth)
{
	if (BossStatusWidgetInstance)
	{
		BossStatusWidgetInstance->UpdateBossHPBar(InCurrentHealth, InMaxHealth);
	}
}

void AHKT_Player::ServerBossWidgetUpdate_Implementation()
{
	if (IsLocallyControlled() && BossStatusWidgetClass)
	{
		BossStatusWidgetInstance = CreateWidget<UBossStatusWidget>(GetWorld(), BossStatusWidgetClass);
		if (BossStatusWidgetInstance)
		{
			BossStatusWidgetInstance->AddToViewport();
			BossStatusWidgetInstance->UpdateBossName(FString::Printf(TEXT("Warden of the Twisted Forest")));	// 보스 네임
		}
	}
}

// Called when the game starts or when spawned
void AHKT_Player::BeginPlay()
{
	Super::BeginPlay();

	// 플레이어 컨트롤러 가져오기
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (PC)
	{
		// 게임 전용 입력 모드로 설정하고 마우스 커서 숨김
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = false;
	}

	

}

// Called every frame
void AHKT_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// 키 입력 바인딩을 설정
void AHKT_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AHKT_Player::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AHKT_Player::MoveRight);

	// 기본 마우스 회전 입력: 직접 컨트롤러에 회전값을 전달
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	// 점프 입력: 눌렀을 때 점프, 떼면 점프 중지
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
}

// 
void AHKT_Player::MoveForward(float Value)
{
	// Controller 값이 Null이 아니고, 방향키 입력이 이루어졌을 때

	if (Controller && Value != 0.0f)
	{
		// 컨트롤러의 회전값에서 Pitch, Roll을 0으로 만들어 Yaw만 사용
		FRotator ControlRot = Controller->GetControlRotation();
		ControlRot.Pitch = 0.f;
		ControlRot.Roll = 0.f;

		// 회전값에서 Forward(X축) 벡터 구하기
		FVector ForwardVector = UKismetMathLibrary::GetForwardVector(ControlRot);

		// AddMovementInput으로 캐릭터 이동
		AddMovementInput(ForwardVector, Value);
	}
}

void AHKT_Player::MoveRight(float Value)
{
	// Controller 값이 Null이 아니고, 방향키 입력이 이루어졌을 때
	if (Controller && Value != 0.0f)
	{
		// 컨트롤러의 회전값에서 Pitch, Roll을 0으로 만들어 Yaw만 사용
		FRotator ControlRot = Controller->GetControlRotation();
		ControlRot.Pitch = 0.f;
		ControlRot.Roll = 0.f;

		// 회전값에서 Right(Y축) 벡터 구하기
		FVector RightVector = UKismetMathLibrary::GetRightVector(ControlRot);

		// AddMovementInput으로 캐릭터 이동
		AddMovementInput(RightVector, Value);
	}
}

void AHKT_Player::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void AHKT_Player::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

