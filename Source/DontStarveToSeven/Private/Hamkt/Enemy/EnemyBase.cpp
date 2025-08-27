// Fill out your copyright notice in the Description page of Project Settings.


#include "Hamkt/Enemy/EnemyBase.h"
#include "Hamkt/Enemy/AI/BPI_EnemyAI.h"
#include "Hamkt/Enemy/AI/AIC_EnemyBase.h"
#include "Hamkt/Widget/EnemyHealthBar.h"

#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"	// GetCharacterMovement() ��ȯŸ���� ����ϱ� ���� �ʼ�

#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Blueprint/UserWidget.h"

#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavigationSystem.h"      // UNavigationSystemV1 ���� �Լ� ���

#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"	// �α� �� ��Ÿ ���� �Լ�
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"

#include "Kismet/KismetSystemLibrary.h"


// Sets default values
AEnemyBase::AEnemyBase()
{
	Tags.Add(TEXT("Enemy"));
	

	AutoPossessAI = EAutoPossessAI::PlacedInWorld;

	// Pawn�� ����� AIController Ŭ������ ��������� �����մϴ�.
	AIControllerClass = AAIC_EnemyBase::StaticClass();

	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// ĸ�� ������Ʈ ũ�� ����
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	// 42.f: ĸ�� �ݰ�, 96.f: ĸ�� �ݳ��� (�ʿ信 ���� �� ����)

	// ���̷��� �Ž��� ��� ��ġ�� ĸ���� �°� ����
	// ���� ĳ������ �޽��� ĸ���� �߾Ӻ��� ���� �����Ƿ�, �Ʒ��� �������� �־� ĸ���� �ڿ������� ����ϴ�.
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -96.f));

	// Z�� Yaw ���� ȸ��
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	// �Ž� ������ ���� (�ʿ� �� ����)
	GetMesh()->SetWorldScale3D(FVector(1.f, 1.f, 1.f));

	// �÷��̾�� ī�޶� �浹 ����. ī�޶� Collision Ignore 
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = true;

	GetCharacterMovement()->bUseRVOAvoidance = true;
	GetCharacterMovement()->AvoidanceConsiderationRadius = 100.f;


	// ----------------------------------------------------------------
	// HealthBar ���� ����
	HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBarWidgetComponent->SetupAttachment(GetMesh());
	HealthBarWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 200.f));
	HealthBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);	// ��ũ�� ���
	HealthBarWidgetComponent->SetDrawSize(FVector2D(200.f, 20.f));

	//static ConstructorHelpers::FClassFinder<UUserWidget> HealthBarBPClass(TEXT("/Game/Hamkt/Widget/W_EnemyHealthBar"));
	///*
	//   - "/Game/UI/W_HealthBar" �κ��� ���� ������Ʈ �� �������Ʈ ��θ� �ǹ�
	//   - C(��������)�� ġ�� ���� ��� ��� ���ڿ��� �Լ��� �ѱ�� ����
	//*/
	//if (HealthBarBPClass.Succeeded())
	//{
	//	HealthBarClass = HealthBarBPClass.Class;
	//}

}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	AController* CurrentController = GetController();
	if (!CurrentController)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Controller found on %s"), *GetName());
		SpawnDefaultController();
		CurrentController = GetController();
	}


	// AIC_Enemy_Base�� ĳ���� �õ�
	AAIC_EnemyBase* EnemyBaseController = Cast<AAIC_EnemyBase>(CurrentController);
	if (EnemyBaseController)
	{
		// ���� ��, AICEnemyBase ��� ������ ����
		AICEnemyBase = EnemyBaseController;
		UE_LOG(LogTemp, Log, TEXT("AICEnemyBase set on %s"), *GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Controller is not AIC_Enemy_Base on %s"), *GetName());
	}

	///////////////////////////////////////////////

	// HealthBar ����

	if (!IsValid(HealthBarClass)) return;	// ��ȿ�� �˻�

	MyHealthBar = CreateWidget<UEnemyHealthBar>(GetWorld(), HealthBarClass);

	// (1) ���� �� ����(AEnemyBase)�� BPI_EnemyAI�� �����ߴ��� Ȯ��
	if (GetClass()->ImplementsInterface(UBPI_EnemyAI::StaticClass()))
	{
		// (2) �������̽� �����͸� TScriptInterface�� ����
		MyHealthBar->HealthOwner = TScriptInterface<IBPI_EnemyAI>(this);

		if (MyHealthBar && HealthBarWidgetComponent)
		{
			HealthBarWidgetComponent->SetWidget(MyHealthBar);
		}
	}

	HealthBarWidgetComponent->SetHiddenInGame(true);

}


// ĳ������ �ٴ� ����Ÿ�Ժ� �̵��ӵ� ����
void AEnemyBase::SetMovementSpeed_Implementation(E_MovementSpeed NewSpeed)
{
	float SpeedValue = 0.f;	// �̵��ӵ� ��

	// �������� ���� �̵��ӵ� ���� ����
	switch (NewSpeed)
	{
	case E_MovementSpeed::Idle:
		SpeedValue = 0.f;
		break;
	case E_MovementSpeed::Walking:
		SpeedValue = 100.f;
		break;
	case E_MovementSpeed::Jogging:
		SpeedValue = 300.f;
		break;
	case E_MovementSpeed::Sprinting:
		SpeedValue = 500.f;
		break;
	default:
		break;
	}

	// CharacterMovementComponent�� ���� MakWalkSpeed ����
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	if (MoveComp)
	{
		MoveComp->MaxWalkSpeed = SpeedValue;
	}
}

// ���� �ݰ�, ���ݰ� ���ϱ�. Keyward ��Ÿ�
void AEnemyBase::GetIdealRange_Implementation(float& AttackRadius, float& DefendRadius) const
{
	AttackRadius = 100.f;
	DefendRadius = 100.f;
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// AIC�� �� ĳ���� ��Ȳ�� �°� �����̺��Ʈ���� ����
UBehaviorTree* AEnemyBase::GetBehaviorTree()
{
	return BehaviorTree;
}

// ���� �������̽� �Լ�
void AEnemyBase::Attack_Implementation(FName NotifyName)
{
}

void AEnemyBase::ThrowWeaponAtTarget_Implementation(AActor* InWeapon, AActor* InTarget)
{
}


float AEnemyBase::GetCurrentHealth_Implementation()
{
	return Health;
}

float AEnemyBase::GetMaxHealth_Implementation()
{
	return MaxHealth;
}

// �������� ���� ���
void AEnemyBase::TakeDamage_Implementation(const float Amount, AActor* DamageActor)
{
	ServerTakeDamage(Amount, this);
}


void AEnemyBase::ServerTakeDamage_Implementation(const float Amount, AActor* DamageActor)
{
	Health = FMath::Clamp(Health -= Amount, 0.f, MaxHealth);	// ������� �ּ� 0, �ִ� 100���� ����

	// HP�� 0 ���ϸ�
	if (Health <= 0)
	{
		bIsDead = true;	// ĳ���� ����
		Die();
	}
}

bool AEnemyBase::IsDead_Implementation()
{
	return bIsDead;
}

// FOnMontageEnded ��������Ʈ Ÿ���� void(UAnimMontage*, bool) ������ �Լ��� �䱸
	// �𸮾� ������ ���������� ��Ÿ�ְ� ������ ����(�Ǵ� �ߴܵǴ� ����) ���� ������ �����ϸ鼭 Montage�� bInterrupted ���� �� �Լ��� ����
void AEnemyBase::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (bInterrupted)
	{
		UE_LOG(LogTemp, Log, TEXT("Attack montage was interrupted."));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Attack montage completed successfully."));
	}

	OnAttackEnd.Broadcast();	// �̺�Ʈ ����ó ȣ��
}

// ĳ���Ͱ� ���� �� ����
void AEnemyBase::Die()
{
	FString HealthMessage = FString::Printf(TEXT("Health: %d"), Health);
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, HealthMessage);
	}
	ServerDie();
}

void AEnemyBase::ShowHealthBarForDuration(float Duration)
{
	if (HealthBarWidgetComponent)
	{
		GetWorld()->GetTimerManager().ClearTimer(HpBarTimerHandle);

		HealthBarWidgetComponent->SetHiddenInGame(false);

		// ����� �޽����� Ȯ��
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, Duration, FColor::Green, TEXT("HealthBarWidgetComponent is shown"));
		}

		GetWorldTimerManager().SetTimer(
			HpBarTimerHandle,
			[this]()
			{
				if (HealthBarWidgetComponent)
				{
					// ���� ��� ������ ����� �ʿ� �� Component�� Destroy�� ���� �ֽ��ϴ�.
					HealthBarWidgetComponent->SetHiddenInGame(true);
					if (GEngine)
					{
						GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("HealthBarWidgetComponent is hidden"));
					}
				}
			},
			Duration,
			false
		);
	}
}



void AEnemyBase::ServerDie_Implementation()
{
	MulticastDie();
}

void AEnemyBase::MulticastDie_Implementation()
{
	USkeletalMeshComponent* MyMesh = GetMesh();
	
	if (MyMesh)
	{
		// 1) ���� �ùķ��̼� �ѱ�
		MyMesh->SetSimulatePhysics(true);

		// 2) �浹 ��带 Query and Physics�� ����
		MyMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		// 5) HP Component ����
		//HealthBarWidgetComponent->SetHiddenInGame(true);
		HealthBarWidgetComponent->DestroyComponent();

		if (HasAuthority()) {
			// 3) ��� ���� �ߴ�
			AICEnemyBase->BrainComponent->StopLogic(TEXT(""));
			// Enemy tag ��� ����
			Tags.Empty();

			// 4) State ���¸� Dead�� ��ȯ.
			AICEnemyBase->SetStateAsDead();
		}
	}
}


void AEnemyBase::ServerAttackMontage_Implementation(const FName& NotifyName)
{

}

void AEnemyBase::MulticastAttackMontage_Implementation(const FName& NotifyName)
{

}


void AEnemyBase::ServerThrowWeaponAtTarget_Implementation(AActor* InWeapon, AActor* InTarget)
{
}

void AEnemyBase::MulticastThrowWeaponAtTarget_Implementation(AActor* InWeapon, AActor* InTarget)
{
}



void AEnemyBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEnemyBase, MaxHealth);
	DOREPLIFETIME(AEnemyBase, Health);
	DOREPLIFETIME(AEnemyBase, bIsDead);
}

