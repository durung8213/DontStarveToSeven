// Fill out your copyright notice in the Description page of Project Settings.


#include "Hamkt/Enemy/EnemyBase.h"
#include "Hamkt/Enemy/AI/BPI_EnemyAI.h"
#include "Hamkt/Enemy/AI/AIC_EnemyBase.h"
#include "Hamkt/Widget/EnemyHealthBar.h"

#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"	// GetCharacterMovement() 반환타입을 사용하기 위해 필수

#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Blueprint/UserWidget.h"

#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavigationSystem.h"      // UNavigationSystemV1 관련 함수 사용

#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"	// 로깅 및 기타 헬퍼 함수
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"

#include "Kismet/KismetSystemLibrary.h"


// Sets default values
AEnemyBase::AEnemyBase()
{
	Tags.Add(TEXT("Enemy"));
	

	AutoPossessAI = EAutoPossessAI::PlacedInWorld;

	// Pawn이 사용할 AIController 클래스를 명시적으로 지정합니다.
	AIControllerClass = AAIC_EnemyBase::StaticClass();

	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// 캡슐 컴포넌트 크기 조정
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	// 42.f: 캡슐 반경, 96.f: 캡슐 반높이 (필요에 따라 값 조정)

	// 스켈레톤 매쉬의 상대 위치를 캡슐에 맞게 조정
	// 보통 캐릭터의 메쉬는 캡슐의 중앙보다 위에 있으므로, 아래로 오프셋을 주어 캡슐과 자연스럽게 맞춥니다.
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -96.f));

	// Z축 Yaw 기준 회전
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	// 매쉬 스케일 조정 (필요 시 변경)
	GetMesh()->SetWorldScale3D(FVector(1.f, 1.f, 1.f));

	// 플레이어와 카메라 충돌 방지. 카메라 Collision Ignore 
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = true;

	GetCharacterMovement()->bUseRVOAvoidance = true;
	GetCharacterMovement()->AvoidanceConsiderationRadius = 100.f;


	// ----------------------------------------------------------------
	// HealthBar 위젯 설정
	HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBarWidgetComponent->SetupAttachment(GetMesh());
	HealthBarWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 200.f));
	HealthBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);	// 스크린 방식
	HealthBarWidgetComponent->SetDrawSize(FVector2D(200.f, 20.f));

	//static ConstructorHelpers::FClassFinder<UUserWidget> HealthBarBPClass(TEXT("/Game/Hamkt/Widget/W_EnemyHealthBar"));
	///*
	//   - "/Game/UI/W_HealthBar" 부분은 실제 프로젝트 내 블루프린트 경로를 의미
	//   - C(절차지향)로 치면 파일 경로 상수 문자열을 함수에 넘기는 느낌
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


	// AIC_Enemy_Base로 캐스팅 시도
	AAIC_EnemyBase* EnemyBaseController = Cast<AAIC_EnemyBase>(CurrentController);
	if (EnemyBaseController)
	{
		// 성공 시, AICEnemyBase 멤버 변수에 저장
		AICEnemyBase = EnemyBaseController;
		UE_LOG(LogTemp, Log, TEXT("AICEnemyBase set on %s"), *GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Controller is not AIC_Enemy_Base on %s"), *GetName());
	}

	///////////////////////////////////////////////

	// HealthBar 설정

	if (!IsValid(HealthBarClass)) return;	// 유효성 검사

	MyHealthBar = CreateWidget<UEnemyHealthBar>(GetWorld(), HealthBarClass);

	// (1) 현재 이 액터(AEnemyBase)가 BPI_EnemyAI를 구현했는지 확인
	if (GetClass()->ImplementsInterface(UBPI_EnemyAI::StaticClass()))
	{
		// (2) 인터페이스 포인터를 TScriptInterface로 세팅
		MyHealthBar->HealthOwner = TScriptInterface<IBPI_EnemyAI>(this);

		if (MyHealthBar && HealthBarWidgetComponent)
		{
			HealthBarWidgetComponent->SetWidget(MyHealthBar);
		}
	}

	HealthBarWidgetComponent->SetHiddenInGame(true);

}


// 캐릭터의 뛰는 열거타입별 이동속도 지정
void AEnemyBase::SetMovementSpeed_Implementation(E_MovementSpeed NewSpeed)
{
	float SpeedValue = 0.f;	// 이동속도 값

	// 열거형에 따라 이동속도 값을 설정
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

	// CharacterMovementComponent를 통해 MakWalkSpeed 변경
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	if (MoveComp)
	{
		MoveComp->MaxWalkSpeed = SpeedValue;
	}
}

// 공격 반경, 방어반경 정하기. Keyward 사거리
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

// AIC에 각 캐릭터 상황에 맞게 비헤이비어트리를 설정
UBehaviorTree* AEnemyBase::GetBehaviorTree()
{
	return BehaviorTree;
}

// 공격 인터페이스 함수
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

// 데미지를 입을 경우
void AEnemyBase::TakeDamage_Implementation(const float Amount, AActor* DamageActor)
{
	ServerTakeDamage(Amount, this);
}


void AEnemyBase::ServerTakeDamage_Implementation(const float Amount, AActor* DamageActor)
{
	Health = FMath::Clamp(Health -= Amount, 0.f, MaxHealth);	// 결과값은 최소 0, 최대 100으로 설정

	// HP가 0 이하면
	if (Health <= 0)
	{
		bIsDead = true;	// 캐릭터 죽음
		Die();
	}
}

bool AEnemyBase::IsDead_Implementation()
{
	return bIsDead;
}

// FOnMontageEnded 델리게이트 타입은 void(UAnimMontage*, bool) 형식의 함수를 요구
	// 언리얼 엔진이 내부적으로 몽타주가 끝나는 순간(또는 중단되는 순간) 다음 로직을 실행하면서 Montage와 bInterrupted 값을 이 함수에 전달
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

	OnAttackEnd.Broadcast();	// 이벤트 디스패처 호출
}

// 캐릭터가 죽을 때 실행
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

		// 디버그 메시지로 확인
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
					// 제거 대신 위젯을 숨기고 필요 시 Component를 Destroy할 수도 있습니다.
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
		// 1) 물리 시뮬레이션 켜기
		MyMesh->SetSimulatePhysics(true);

		// 2) 충돌 모드를 Query and Physics로 설정
		MyMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		// 5) HP Component 제거
		//HealthBarWidgetComponent->SetHiddenInGame(true);
		HealthBarWidgetComponent->DestroyComponent();

		if (HasAuthority()) {
			// 3) 모든 로직 중단
			AICEnemyBase->BrainComponent->StopLogic(TEXT(""));
			// Enemy tag 모두 제거
			Tags.Empty();

			// 4) State 상태를 Dead로 전환.
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

