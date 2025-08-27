// Fill out your copyright notice in the Description page of Project Settings.


#include "Hamkt/Player/HKT_Player.h"
#include "GameFramework/Controller.h"			// ��Ʈ�ѷ� ���� �Լ� ���
#include "GameFramework/SpringArmComponent.h"	// �������� ������Ʈ ���
#include "GameFramework/CharacterMovementComponent.h"	// GetCharacterMovement() ��ȯŸ���� ����ϱ� ���� �ʼ�
#include "Components/InputComponent.h"			// �Է� ó�� ���� ���
#include "Engine/World.h"						// GetWorld() �Լ� ���
#include "Camera/CameraComponent.h"				// ī�޶� ������Ʈ ���
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Hamkt/Widget/BossStatusWidget.h"
#include "Hamkt/Enemy/EnemyBoss/EnemyBossBase.h"

// Sets default values
AHKT_Player::AHKT_Player()
{
 	// �� ������ ƽ�Լ� ȣ�� ���
	PrimaryActorTick.bCanEverTick = true;

	// ĳ���Ͱ� ��Ʈ�ѷ��� ȸ������ ���� ���
	bUseControllerRotationYaw = false;		// Yaw (�¿� ȸ��) x
	bUseControllerRotationPitch = false;	// Pitch(���� ȸ��)�� ĳ���Ϳ� ���� x
	bUseControllerRotationRoll = false;		// Roll(�¿� ����)�� ���� x

	// Mesh() Location ����
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -96.f));

	// Mesh() Rotation ����
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	// �̵��������� ĳ���� ������ ȸ��
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ĳ���� ȸ������
	GetCharacterMovement()->MaxWalkSpeed = 800.0f;

	// �������� ������Ʈ ���� �� �ʱ�ȭ
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);		// ���������� ��Ʈ ������Ʈ�� ����
	CameraBoom->TargetArmLength = 300.f;			// ī�޶�� ĳ���� ���� �⺻ �Ÿ� ���� (300 unit)
	CameraBoom->bUsePawnControlRotation = true;		// ��Ʈ�ѷ��� ȸ������ �������Ͽ� �ݿ�

	// ī�޶� ������Ʈ ���� �� �ʱ�ȭ
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));	// ����
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);		// ī�޶� �������Ͽ� ����.
	FollowCamera->bUsePawnControlRotation = false;									// ī�޶� ��ü ȸ�� ���� (���������� ���ư��� ����)

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
			BossStatusWidgetInstance->UpdateBossName(FString::Printf(TEXT("Warden of the Twisted Forest")));	// ���� ����
		}
	}
}

// Called when the game starts or when spawned
void AHKT_Player::BeginPlay()
{
	Super::BeginPlay();

	// �÷��̾� ��Ʈ�ѷ� ��������
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (PC)
	{
		// ���� ���� �Է� ���� �����ϰ� ���콺 Ŀ�� ����
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

// Ű �Է� ���ε��� ����
void AHKT_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AHKT_Player::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AHKT_Player::MoveRight);

	// �⺻ ���콺 ȸ�� �Է�: ���� ��Ʈ�ѷ��� ȸ������ ����
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	// ���� �Է�: ������ �� ����, ���� ���� ����
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
}

// 
void AHKT_Player::MoveForward(float Value)
{
	// Controller ���� Null�� �ƴϰ�, ����Ű �Է��� �̷������ ��

	if (Controller && Value != 0.0f)
	{
		// ��Ʈ�ѷ��� ȸ�������� Pitch, Roll�� 0���� ����� Yaw�� ���
		FRotator ControlRot = Controller->GetControlRotation();
		ControlRot.Pitch = 0.f;
		ControlRot.Roll = 0.f;

		// ȸ�������� Forward(X��) ���� ���ϱ�
		FVector ForwardVector = UKismetMathLibrary::GetForwardVector(ControlRot);

		// AddMovementInput���� ĳ���� �̵�
		AddMovementInput(ForwardVector, Value);
	}
}

void AHKT_Player::MoveRight(float Value)
{
	// Controller ���� Null�� �ƴϰ�, ����Ű �Է��� �̷������ ��
	if (Controller && Value != 0.0f)
	{
		// ��Ʈ�ѷ��� ȸ�������� Pitch, Roll�� 0���� ����� Yaw�� ���
		FRotator ControlRot = Controller->GetControlRotation();
		ControlRot.Pitch = 0.f;
		ControlRot.Roll = 0.f;

		// ȸ�������� Right(Y��) ���� ���ϱ�
		FVector RightVector = UKismetMathLibrary::GetRightVector(ControlRot);

		// AddMovementInput���� ĳ���� �̵�
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

