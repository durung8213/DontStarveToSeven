// Fill out your copyright notice in the Description page of Project Settings.


#include "Kimmh/BasePlayerCharacter.h"
#include "Kimmh/Camera/BaseCamera.h"
#include "Kimmh/Character/PlayerState/DTS_BasePlayerState.h"
#include "Kimmh/Enum/IkUseEnum.h"
#include "Kimmh/HUDnUI/UI/DTS_BaseInteractionUserWidget.h"
#include "Kimmh/Actor/DTS_BaseTool.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Seojy/CraftManager.h"
#include "Leesh/SHBaseItem.h"
#include "Leesh/SHWeaponItem.h"
#include "Hamkt/Widget/BossStatusWidget.h"
#include "Hamkt/Enemy/EnemyBoss/EnemyBossBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"

ABasePlayerCharacter::ABasePlayerCharacter()
{

	// 스프링 암 달기
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));	// 스프링 암 생성
	CameraBoom->SetupAttachment(RootComponent);										// 스프링 암을 루트에 부착
	CameraBoom->TargetArmLength = 250.f;											// 스프링 암 거리 조절
	CameraBoom->SetRelativeLocation(FVector(0, 0, 60));								// 스트링 암 상대 위치 조정
	CameraBoom->bUsePawnControlRotation = true;										// 컨트롤러 로테이션 켬

	// 카메라 없음을 방지하기 위한 보조 카메라
	ExtraCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ExtraCamera"));
	ExtraCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	ExtraCamera->bUsePawnControlRotation = false;

	ThirdPersonBaseCameraActor = CreateDefaultSubobject<ABaseCamera>(TEXT("ThirdPersonBaseCameraActor"));
	ThirdPersonBaseCameraActor->AttachToComponent(CameraBoom, FAttachmentTransformRules::SnapToTargetNotIncludingScale, USpringArmComponent::SocketName);
	ThirdPersonBaseCameraActor->Camera->bUsePawnControlRotation = false;

	/*static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/Kimmh/Character/Animation/ABP_BaseCharacter.ABP_BaseCharacter_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("애님 블루프린트 설정 실패"));
	}*/

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;	// 컨트롤러의 회전값에 따라 캐릭터 역시 회전하게 함
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);	// 위에걸로 회전하는 회전 반경

	// ABase카메라를 기본 카메라로 사용하기 위한 액터 컴포넌트 생성
	CameraComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("CameraComponent")); // 자식 액터 컴포넌트 생성
	CameraComponent->SetupAttachment(CameraBoom); // CameraBoom에 부착
	CameraComponent->SetChildActorClass(ABaseCamera::StaticClass()); // ABaseCamera를 Child Actor로 설정

	// 기본 변수 초기화
	ComboCount = 0;
	UpperBodyOverlay = EUpperOverlay::Spear;
	bNormalAttackRequest = false;
	bStrongAttackRequest = false;
	bIsAiming = false;
	bCanAttack = true;

	DrawAmount = 0;
	Yaw = 0;
	Pitch = 0;
	MoveX = 0;
	MoveY = 0;

	// FullBody 세션 이름 추가
	FullBodySectionName = {
		FName(TEXT("SpearComboAttack1")),
		FName(TEXT("SpearComboAttack2")),
		FName(TEXT("SpearComboAttack3")),
		FName(TEXT("SpearStrongAttack1")),
		FName(TEXT("OneHandSwordCombo1")),
		FName(TEXT("OneHandSwordCombo2")),
		FName(TEXT("OneHandSwordCombo3")),
		FName(TEXT("OneHandSwordStrongAttack")),
		FName(TEXT("TwoHandWeaponAttack1")),
		FName(TEXT("TwoHandWeaponAttack2")),
		FName(TEXT("TwoHandWeaponAttack3")),
		FName(TEXT("TwoHandWeaponStrongAttack")),
	};

	// UpperBody 세션 이름 추가
	UpperBodySectionName = {
		FName(TEXT("SpearJumpAttack1")),
		FName(TEXT("OneHandSwordAirAttack")),
		FName(TEXT("TwoHandSwordAirAttack")),
	};

	HitSectionName = {
		FName(TEXT("HitFront")),
		FName(TEXT("HitBack")),
		FName(TEXT("HitLeft")),
		FName(TEXT("HitRight")),

	};

	InteractionSectionName = {
		FName(TEXT("WoodAxeEquip")),
		FName(TEXT("WoodAxeSwing1")),
		FName(TEXT("WoodAxeSwing2")),
		FName(TEXT("WoodAxeSwing3")),
		FName(TEXT("PickAexEquip")),
		FName(TEXT("PickAxeSwingLow")),
		FName(TEXT("PickAxeSwingMiddle")),
		FName(TEXT("PickAxeSwingHigh")),
		FName(TEXT("UnEquip")),
		FName(TEXT("HarvestBush")),
		FName(TEXT("PickUpItemGround")),
		FName(TEXT("PickUpItemStanding")),
	};

	CraaftManager = CreateDefaultSubobject<UCraftManager>(TEXT("CreaftManager"));

	PickAxeMid = CreateDefaultSubobject<USceneComponent>(TEXT("PickAxeMid"));
	PickAxeHigh = CreateDefaultSubobject<USceneComponent>(TEXT("PickAxeHigh"));
	
	PickAxeMid->SetupAttachment(GetMesh());
	PickAxeMid->SetRelativeLocation(FVector(0, 0, 100));

	PickAxeHigh->SetupAttachment(GetMesh());
	PickAxeMid->SetRelativeLocation(FVector(0, 0, 140));

	WoodAxe1 = CreateDefaultSubobject<USceneComponent>(TEXT("WoodAxe1"));
	WoodAxe2 = CreateDefaultSubobject<USceneComponent>(TEXT("WoodAxe2"));

	WoodAxe1->SetupAttachment(GetMesh());
	//WoodAxe1->SetRelativeLocation(FVector(0, 0, 100));

	WoodAxe2->SetupAttachment(GetMesh());
	//PickAxeMid->SetRelativeLocation(FVector(0, 0, 140));

	// 생존 장비 컴포넌트
	WoodAxeActorComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("WoodAxeActorComponent")); // 자식 액터 컴포넌트 생성
	WoodAxeActorComponent->SetupAttachment(GetMesh(), TEXT("hand_r")); // 메시에 부착
	static ConstructorHelpers::FClassFinder<ADTS_BaseTool> WoodAxeBP(TEXT("/Game/Kimmh/Blueprint/Item/Tool/BP_WoodAxe"));
	if (WoodAxeBP.Succeeded())
	{
		WoodAxeActorComponent->SetChildActorClass(WoodAxeBP.Class); 
		UE_LOG(LogTemp, Log, TEXT("BP_WoodAxe Call Successed"));
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("BP_WoodAxe Call Fail"));
	}

	PickAxeActorComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("PickAxeActorComponent")); // 자식 액터 컴포넌트 생성
	PickAxeActorComponent->SetupAttachment(GetMesh(), TEXT("hand_r")); // 메시에 부착
	static ConstructorHelpers::FClassFinder<ADTS_BaseTool> PickAxeBP(TEXT("/Game/Kimmh/Blueprint/Item/Tool/BP_PickAxe"));
	if (PickAxeBP.Succeeded())
	{
		PickAxeActorComponent->SetChildActorClass(PickAxeBP.Class);
	}

	SickleActorComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("SickleActorComponent")); // 자식 액터 컴포넌트 생성
	SickleActorComponent->SetupAttachment(GetMesh(), TEXT("hand_r")); // 메시에 부착
	static ConstructorHelpers::FClassFinder<ADTS_BaseTool> SickleBP(TEXT("/Game/Kimmh/Blueprint/Item/Tool/BP_Sickle"));
	if (SickleBP.Succeeded())
	{
		SickleActorComponent->SetChildActorClass(SickleBP.Class);
	}

	


	//AEnemyBossBase* Boss = Cast<AEnemyBossBase>(UGameplayStatics::GetActorOfClass(GetWorld(), AEnemyBossBase::StaticClass()));
	//if (Boss)
	//{
	//	Boss->OnTakeDamage.AddDynamic(this, &ABasePlayerCharacter::UpdateBossHPBar);
	//}


}

/// <summary>
/// 자세한 설명은 생략한다
/// </summary>
void ABasePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	/*ComboCount = 0;
	UpperBodyOverlay = EUpperOverlay::Spear;*/
	bCanAttack = true;
	//ThirdPersonBaseCameraActor = GetWorld()->SpawnActor<ABaseCamera>(ABaseCamera::StaticClass());
	//GetWorld()->GetTimerManager().SetTimer(BaseCameraBlendTimerHandle, this, &ABasePlayerCharacter::ChangeCameraToBaseCamera, 0.9f, false);

	if (GetController()) {
		APlayerController* CurrentPlayerController = Cast<APlayerController>(GetController());	// 플레이어 컨트롤러로 변환
		if (CurrentPlayerController) {
			CurrentPlayerController->bAutoManageActiveCameraTarget = false;
			CurrentPlayerController->SetViewTargetWithBlend(CameraComponent->GetChildActor());
		}
	}
	else {
		FString Message = FString::Printf(TEXT("BeginPlay에서 플레이어 컨트롤러 가져오기 실패"));
		UKismetSystemLibrary::PrintString(this, Message, true, true, FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));
	}

	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ABasePlayerCharacter::OnHit);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ABasePlayerCharacter::OnCapsuleBeginOverlap);


	if (WoodAxeActorComponent->GetChildActor()) {
		// 자식 액터가 있으면
		WoodAxe = Cast<ADTS_BaseTool>(WoodAxeActorComponent->GetChildActor());
		UKismetSystemLibrary::PrintString(this, TEXT("도끼 가져오기 성공"));
	}
	else {
		UKismetSystemLibrary::PrintString(this, TEXT("도끼 가져오기 실패"));
	}

	if (PickAxeActorComponent->GetChildActor()) {
		// 자식 액터가 있으면
		PickAxe = Cast<ADTS_BaseTool>(PickAxeActorComponent->GetChildActor());
	}
	else {
		UKismetSystemLibrary::PrintString(this, TEXT("곡괭이 가져오기 실패"));
	}

	if (SickleActorComponent->GetChildActor()) {
		// 자식 액터가 있으면
		Sickle = Cast<ADTS_BaseTool>(SickleActorComponent->GetChildActor());
	}
	else {
		UKismetSystemLibrary::PrintString(this, TEXT("시클 가져오기 실패"));
	}
}

/// <summary>
/// 이게 설명이 필요하면 안되는걸로 압니다
/// </summary>
/// <param name="DeltaTime"></param>
void ABasePlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CamreaForwardVector = CameraComponent->GetForwardVector();

	if (GetController()) {
		if (!HasAuthority()) {
			FRotator BaseAim = GetBaseAimRotation();
			ServerSetPitch(BaseAim.Pitch);
			ServerSetYaw(BaseAim.Yaw);
		}
	}
}


float ABasePlayerCharacter::GetCurrentHealth_Implementation()
{
	ADTS_BasePlayerState* CurrentPlayerState = Cast<ADTS_BasePlayerState>(GetPlayerState());
	if (CurrentPlayerState) {
		return CurrentPlayerState->GetCurrentHp();
	}
	else
	{
		return 0.0f;

	}
}

float ABasePlayerCharacter::GetMaxHealth_Implementation()
{
	ADTS_BasePlayerState* CurrentPlayerState = Cast<ADTS_BasePlayerState>(GetPlayerState());
	if (CurrentPlayerState) {
		return CurrentPlayerState->GetMaxHp();
	}
	else
	{
		return 0.0f;

	}
}


void ABasePlayerCharacter::TakeDamage_Implementation(float Amount, AActor* DamageActor)
{
	// 애니메이션 출력
	if (HasAuthority()) {
		ServerHit();

		ADTS_BasePlayerState* CurrentPlayerState = Cast<ADTS_BasePlayerState>(GetPlayerState());
		if (CurrentPlayerState) {
			CurrentPlayerState->ServerTakeDamage(Amount, this);
		}
		else {
			// CurrentPlayerState->ServerTakeDamage(0);
		}
	}

}

void ABasePlayerCharacter::SeverTakeBossDamage_Implementation(float Amount, AActor* DamageActor)
{
	ServerHit();

	ADTS_BasePlayerState* CurrentPlayerState = Cast<ADTS_BasePlayerState>(GetPlayerState());
	if (CurrentPlayerState) {
		CurrentPlayerState->ServerTakeDamage(Amount, this);
	}
}

bool ABasePlayerCharacter::IsDead_Implementation()
{
	return false;
}

// 콤보를 리셋 하는 함수
void ABasePlayerCharacter::ResetComboCount()
{
	if (IsValid(this)) {
		if (HasAuthority()) {
			ComboCount = 0;
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Invalid object reference!"));
	}
}

// 공격불가상태에서 공격 가능 상태로 변경하는 함수
void ABasePlayerCharacter::ResetbCanAttack()
{
	if (IsValid(this)) {
		if (HasAuthority()) {
			bCanAttack = true;
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Invalid object reference!"));
	}

}

void ABasePlayerCharacter::ResetbIsAiming()
{
	if (IsValid(this)) {
		if (HasAuthority()) {
			bIsAiming = false;
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Invalid object reference!"));
	}
}

void ABasePlayerCharacter::ServerMontagePlayWithSession_Implementation(UAnimMontage* Montage, FName SectionName, float PlayRate)
{
	MultiMontagePlayWithSession(Montage, SectionName, PlayRate);
}

void ABasePlayerCharacter::MultiMontagePlayWithSession_Implementation(UAnimMontage* Montage, FName SectionName, float PlayRate)
{
	PlayMontageWithSession(Montage, SectionName, PlayRate);
}

/// <summary>
/// 몽타주를 재생하는 함수
/// </summary>
/// <param name="Montage">재생할 몽타주</param>
/// <param name="SectionName">세션 이름</param>
void ABasePlayerCharacter::PlayMontageWithSession(UAnimMontage* Montage, FName SectionName, float PlayRate)
{
	FString Message;
	UKismetSystemLibrary::PrintString(this, TEXT("몽타주 실행"));
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		if (Montage && Montage->IsValidSectionName(SectionName)) // AttackMontage는 UAnimMontage* 변수
		{
			AnimInstance->Montage_Play(Montage, PlayRate);
			AnimInstance->Montage_JumpToSection(SectionName, Montage);

		}
		else {
			Message = FString::Printf(TEXT("받아온 몽타주 없음"));
			UKismetSystemLibrary::PrintString(this, Message);
		}
	}
	else {
		Message = FString::Printf(TEXT("AnimInstance 가져오기 실패"));
		UKismetSystemLibrary::PrintString(this, Message);
	}
}

void ABasePlayerCharacter::ResetAttackRequest()
{
	bStrongAttackRequest = false;
	bNormalAttackRequest = false;
}

// 일반 공격을 하는 함수 (서버)
void ABasePlayerCharacter::ServerAttack_Implementation()
{
	if (!HasAuthority()) {
		FString Message = FString::Printf(TEXT("약공격 함수가 서버에서 실행되지 않음"));
		UKismetSystemLibrary::PrintString(this, Message);
	}
	else {
		MultiAttack();
	}

}

// 일반 공격를 하는 함수 (멀티)
void ABasePlayerCharacter::MultiAttack_Implementation()
{
	FString Message;
	FName SectionName;

	bStrongAttackRequest = false;
	bNormalAttackRequest = true;

	if (bCanAttack) {
		// 공격 가능 상태이고
		if (ComboCount == 0) {
			// 콤보 카운트가 0이면
			if (GetCharacterMovement()->IsFalling()) {
				// 공중이면 공중 공격 애니메이션 출력
				if (bCanAttack) {
					switch (UpperBodyOverlay)
					{
					case EUpperOverlay::TwoHandSword:
						PlayMontageWithSession(UpperBodyMontage, UpperBodySectionName[2]);
						break;
					case EUpperOverlay::OneHandSword:
						PlayMontageWithSession(UpperBodyMontage, UpperBodySectionName[1]);
						break;
					case EUpperOverlay::Spear:
						PlayMontageWithSession(UpperBodyMontage, UpperBodySectionName[0]);
						break;
					case EUpperOverlay::Bow:
						break;
					case EUpperOverlay::Pistol:
						break;
					case EUpperOverlay::Rifle:
						break;
					case EUpperOverlay::Normal:
						break;
					default:
						break;
					}
				}
			}
			else {
				switch (UpperBodyOverlay)							// 들고 있는 무기의 종류에 따라
				{
				case EUpperOverlay::OneHandSword:
					UKismetSystemLibrary::PrintString(this, TEXT("한손 무기 콤보 공격 시도"));
					PlayMontageWithSession(MontageArr[0], FName(TEXT("N")), 1.f);
					ResetAttackRequest();
					break;
				case EUpperOverlay::TwoHandSword:
					UKismetSystemLibrary::PrintString(this, TEXT("양손 무기 콤보 공격 시도"));
					PlayMontageWithSession(MontageArr[1], FName(TEXT("N")), 1.f);
					ResetAttackRequest();
					break;
				case EUpperOverlay::Spear:
					UKismetSystemLibrary::PrintString(this, TEXT("스피어 콤보 공격 시도"));
					PlayMontageWithSession(MontageArr[2], FName(TEXT("N")),1.f);
					ResetAttackRequest();
					break;
				case EUpperOverlay::Bow:
					break;
				case EUpperOverlay::Pistol:
					break;
				case EUpperOverlay::Rifle:
					break;
				case EUpperOverlay::Normal:
					return;
					break;
				default:
					break;
				}

				if (HasAuthority()) {
					ComboCount++;
				}

				
			}
		}
	}
	else {
		Message = FString::Printf(TEXT("bCanAttack가 false임"));
		UKismetSystemLibrary::PrintString(this, Message);
	}
}

// 서버에서 강공격을 하라고 지시하는 함수
void ABasePlayerCharacter::ServerStrongAttack_Implementation()
{
	if (!HasAuthority()) {
		FString Message = FString::Printf(TEXT("강공격 함수가 서버에서 실행되지 않음"));
		UKismetSystemLibrary::PrintString(this, Message);
	}
	else {
		MultiStrongAttack();
	}
}

// 멀티에서 강공격을 하라고 지시하는 함수
void ABasePlayerCharacter::MultiStrongAttack_Implementation()
{
	FString Message;
	FName SectionName;

	bStrongAttackRequest = true;
	bNormalAttackRequest = false;

	if (bCanAttack) {
		// 공격 가능 상태이고
		if (ComboCount == 0) {
			// 콤보 카운트가 0이면
			if (GetCharacterMovement()->IsFalling()) {
				// 공중이면 공중 공격 애니메이션 출력


				switch (UpperBodyOverlay)
				{
				case EUpperOverlay::OneHandSword:
					PlayMontageWithSession(UpperBodyMontage, UpperBodySectionName[1]);
					ResetAttackRequest();
					break;
				case EUpperOverlay::TwoHandSword:
					ResetAttackRequest();
					PlayMontageWithSession(UpperBodyMontage, UpperBodySectionName[2]);
					break;
				case EUpperOverlay::Spear:
					PlayMontageWithSession(MontageArr[6], UpperBodySectionName[0]);
					ResetAttackRequest();
					break;
				case EUpperOverlay::Bow:
					break;
				case EUpperOverlay::Pistol:
					break;
				case EUpperOverlay::Rifle:
					break;
				case EUpperOverlay::Normal:
					break;
				default:
					break;
				}
			}
			else 
			{
				switch (UpperBodyOverlay)							// 들고 있는 무기의 종류에 따라
				{
				case EUpperOverlay::OneHandSword:
					UKismetSystemLibrary::PrintString(this, TEXT("한손 무기 콤보 공격 시도"));
					if (MontageArr.IsValidIndex(0)) {
						PlayMontageWithSession(MontageArr[0], FName(TEXT("S")));
						return;
					}
					break;
				case EUpperOverlay::TwoHandSword:
					UKismetSystemLibrary::PrintString(this, TEXT("양손 무기 콤보 공격 시도"));
					PlayMontageWithSession(MontageArr[1], FName(TEXT("S")));
					if (MontageArr.IsValidIndex(1) && !MontageArr[1]->IsValidSectionName(FName(TEXT("S")))) {
						//PlayMontageWithSession(MontageArr[0], FName(TEXT("S")));
						return;
					}
					PlayMontageWithSession(MontageArr[1], FName(TEXT("S")));
					break;
				case EUpperOverlay::Spear:
					UKismetSystemLibrary::PrintString(this, TEXT("스피어 콤보 공격 시도"));
					PlayMontageWithSession(MontageArr[2], FName(TEXT("S")));
					break;
				case EUpperOverlay::Bow:
					break;
				case EUpperOverlay::Pistol:
					break;
				case EUpperOverlay::Rifle:
					break;
				case EUpperOverlay::Normal:
					// 아이템 사용
					return;
					break;
				default:
					break;
				}

				if (HasAuthority()) {
					ComboCount++;
				}
				
			}
			
		}
	}
	else {
		Message = FString::Printf(TEXT("bCanAttack가 false임"));
		UKismetSystemLibrary::PrintString(this, Message);
	}

	
}

// 조준 하고 있을 때 나타내는 것
void ABasePlayerCharacter::ServerBowAiming_Implementation()
{
	bIsAiming = true;
	DrawAmount += 0.01f;
	SetbOrientRotationToMovement(false);
	/*
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	*/
}

void ABasePlayerCharacter::ServerBowShot_Implementation()
{
	if (bIsAiming) {
		// 리플리케이트 되는 화살 소환 뒤 쏨
		DrawAmount = 0;	// 임시 위와 같음
		//bIsAiming = false; // 임시 활 쏘는 애니메이션에 노티파이로 활을 쏜 뒤 작동할 예정
		//bUseControllerRotationYaw = false;
		//GetCharacterMovement()->bOrientRotationToMovement = true;
		MultiBowShotMontagePlay();
	}
	else {
		FString Message = FString::Printf(TEXT("bIsAiming이 false임"));
		UKismetSystemLibrary::PrintString(this, Message);
	}
}

void ABasePlayerCharacter::MultiBowShotMontagePlay_Implementation()
{
	// 활 쏘는 애니메이션
	PlayMontageWithSession(BowMontage, FName(TEXT("BowAfterIdle")));
}

void ABasePlayerCharacter::ServerBowAimReleased_Implementation()
{
	bIsAiming = false;
	DrawAmount = 0;
	SetbOrientRotationToMovement(true);

	/*bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;*/
}

void ABasePlayerCharacter::ServerChangeUpperOverlay_Implementation(EUpperOverlay UpperOverlay)
{
	UpperBodyOverlay = UpperOverlay;
}

void ABasePlayerCharacter::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetController()) {
		ADTS_GamePlayPlayerController* CurrentPlayerController = Cast<ADTS_GamePlayPlayerController>(GetController());	// 플레이어 컨트롤러로 변환
		if (CurrentPlayerController && OtherActor == CurrentPlayerController->InteractionActor) {
			CurrentPlayerController->bAutoMove = false;

			if (CurrentPlayerController->bIsInteractionReady) {

				UDTS_BaseInteractionUserWidget* CurrentInteractionWidget;

				IDTS_BPIInteraction* InterfaceInstance = Cast<IDTS_BPIInteraction>(CurrentPlayerController->InteractionActor);
				if (InterfaceInstance)
				{
					CurrentInteractionActor = CurrentPlayerController->InteractionActor;
					bInteractionDoing = true;
					FString Message;
					//ASHBaseItem* CurrentItem;
					switch (InterfaceInstance->Execute_GetInteractionType(CurrentPlayerController->InteractionActor))
					{
					case EInteraction::Item:
						UKismetSystemLibrary::PrintString(this, TEXT("줍기 시작"));
						if (Cast<ASHBaseItem>(CurrentInteractionActor)) {
							CurrentItem = Cast<ASHBaseItem>(CurrentInteractionActor);
							Message = FString::Printf(TEXT("캐릭터 : %f, 아이템 : %f"), GetActorLocation().Z + 10.f, CurrentItem->GetActorLocation().Z);
							UKismetSystemLibrary::PrintString(this, Message);
							if (GetActorLocation().Z>= CurrentItem->GetActorLocation().Z) {
								ServerMontagePlayWithSession(InteractionMontage, InteractionSectionName[10], 2.f);
							}
							else {
								ServerMontagePlayWithSession(InteractionMontage, InteractionSectionName[11], 2.f);
							}
						}
						break;
					case EInteraction::Bush:
						ServerMontagePlayWithSession(InteractionMontage, InteractionSectionName[9]);	// 풀 배는 몽타주 실행
						CurrentInteractionWidget = InterfaceInstance->Execute_GetBaseInteractionWidget(CurrentPlayerController->InteractionActor);
						if (CurrentInteractionWidget) {
							CurrentInteractionWidget->SetInputKeyCollapsed();
							CurrentInteractionWidget->SetRoundPorgressBarVisible(1 * InterfaceInstance->Execute_GetInteractionHp(CurrentPlayerController->InteractionActor));
						}
						break;
					case EInteraction::Tree:
						// 나무면
						ServerMontagePlayWithSession(InteractionMontage, InteractionSectionName[0]);	// 도끼를 소환하는 몽타주 실행
						CurrentInteractionWidget = InterfaceInstance->Execute_GetBaseInteractionWidget(CurrentPlayerController->InteractionActor);
						if (CurrentInteractionWidget) {
							CurrentInteractionWidget->SetInputKeyCollapsed();
							CurrentInteractionWidget->SetRoundPorgressBarVisible(0.88 * InterfaceInstance->Execute_GetInteractionHp(CurrentPlayerController->InteractionActor));
						}
						break;
					case EInteraction::Rock:
						ServerMontagePlayWithSession(InteractionMontage, InteractionSectionName[4], 0.8f);	// 곡괭이를 소환하는 몽타주 실행
						CurrentInteractionWidget = InterfaceInstance->Execute_GetBaseInteractionWidget(CurrentPlayerController->InteractionActor);
						if (CurrentInteractionWidget) {
							CurrentInteractionWidget->SetInputKeyCollapsed();
							CurrentInteractionWidget->SetRoundPorgressBarVisible(0.88 * InterfaceInstance->Execute_GetInteractionHp(CurrentPlayerController->InteractionActor));
						}
						break;
					default:
						break;
					}
				}
				CurrentPlayerController->bIsInteractionReady = false;
				CurrentPlayerController->bIsInteractionReady = false;
				UKismetSystemLibrary::PrintString(this, TEXT("상호작용 동작 하는중 시작"));
			}
		}
	}
}

void ABasePlayerCharacter::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (GetController()) {
		ADTS_GamePlayPlayerController* CurrentPlayerController = Cast<ADTS_GamePlayPlayerController>(GetController());	// 플레이어 컨트롤러로 변환
		if (CurrentPlayerController && OtherActor == CurrentPlayerController->InteractionActor) {
			CurrentPlayerController->bAutoMove = false;

			if (CurrentPlayerController->bIsInteractionReady) {
				UDTS_BaseInteractionUserWidget* CurrentInteractionWidget;

				IDTS_BPIInteraction* InterfaceInstance = Cast<IDTS_BPIInteraction>(CurrentPlayerController->InteractionActor);
				if (InterfaceInstance)
				{
					CurrentInteractionActor = CurrentPlayerController->InteractionActor;
					bInteractionDoing = true;
					switch (InterfaceInstance->Execute_GetInteractionType(CurrentPlayerController->InteractionActor))
					{
					case EInteraction::Item:
						UKismetSystemLibrary::PrintString(this, TEXT("줍기 시작"));
						if (Cast<ASHBaseItem>(CurrentInteractionActor)) {
							CurrentItem = Cast<ASHBaseItem>(CurrentInteractionActor);
							ServerMontagePlayWithSession(InteractionMontage, InteractionSectionName[10], 2.f);
						}
						break;
					case EInteraction::Bush:
						ServerMontagePlayWithSession(InteractionMontage, InteractionSectionName[9]);	// 풀 배는 몽타주 실행
						CurrentInteractionWidget = InterfaceInstance->Execute_GetBaseInteractionWidget(CurrentPlayerController->InteractionActor);
						if (CurrentInteractionWidget) {
							CurrentInteractionWidget->SetInputKeyCollapsed();
							CurrentInteractionWidget->SetRoundPorgressBarVisible(1 * InterfaceInstance->Execute_GetInteractionHp(CurrentPlayerController->InteractionActor));
						}
						break;
					case EInteraction::Tree:
						// 나무면
						ServerMontagePlayWithSession(InteractionMontage, InteractionSectionName[0]);	// 도끼를 소환하는 몽타주 실행
						CurrentInteractionWidget = InterfaceInstance->Execute_GetBaseInteractionWidget(CurrentPlayerController->InteractionActor);
						if (CurrentInteractionWidget) {
							CurrentInteractionWidget->SetInputKeyCollapsed();
							CurrentInteractionWidget->SetRoundPorgressBarVisible(0.88* InterfaceInstance->Execute_GetInteractionHp(CurrentPlayerController->InteractionActor));
						}
						break;
					case EInteraction::Rock:
						ServerMontagePlayWithSession(InteractionMontage, InteractionSectionName[4], 0.8f);	// 곡괭이를 소환하는 몽타주 실행
						CurrentInteractionWidget = InterfaceInstance->Execute_GetBaseInteractionWidget(CurrentPlayerController->InteractionActor);
						if (CurrentInteractionWidget) {
							CurrentInteractionWidget->SetInputKeyCollapsed();
							CurrentInteractionWidget->SetRoundPorgressBarVisible(0.88 * InterfaceInstance->Execute_GetInteractionHp(CurrentPlayerController->InteractionActor));
						}
						break;
					default:
						break;
					}
				}
				CurrentPlayerController->bIsInteractionReady = false;
				UKismetSystemLibrary::PrintString(this, TEXT("상호작용 동작 하는중 시작"));
			}
		}
	}
}

void ABasePlayerCharacter::AttachItem(ASHBaseItem* NewEquipItem)
{
	/*FName CustomObjectTypeName(TEXT("Weapon"));
	ECollisionChannel CustomChannel = UCollisionProfile::Get()->ConvertToCollisionChannel(false, CustomObjectTypeName);*/

	DetachItem();	// 아이템 장착 해제
	if (NewEquipItem) {
		NewEquipItem->ServerSetMeshSimulatePhysics(false);
		NewEquipItem->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, NewEquipItem->AttachSocket);	// NewEquipItem->AttachSocket에 해당 아이템 붙임
		NewEquipItem->SetActorRelativeLocation(NewEquipItem->GetAttachLocation());
		NewEquipItem->SetActorRelativeRotation(NewEquipItem->GetAttachRot());
		NewEquipItem->ServerSetCollisionObjectTypeWeapon();
		UE_LOG(LogTemp, Warning, TEXT("Object Type: %d"), NewEquipItem->Collision->GetCollisionObjectType());
		CurrentEquipItem = NewEquipItem;
		IkUse = NewEquipItem->IkUse;
		UpperBodyOverlay = NewEquipItem->UpperBodyOverlay;
	}
}

void ABasePlayerCharacter::DetachItem()
{
	if (CurrentEquipItem) {
		ASHBaseItem* TargetItem = CurrentEquipItem;
		TargetItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		TargetItem->Destroy();
		CurrentEquipItem = nullptr;
		IkUse = EIkUse::None;
		UpperBodyOverlay = EUpperOverlay::Normal;
	}

}

FTransform ABasePlayerCharacter::GetAnotherHandTransform()
{

	FVector OutputPos;
	FRotator OutputRot;

	if (CurrentEquipItem) {
		FTransform SocketTransform = CurrentEquipItem->GetAnotherHandTransform();

		switch (CurrentEquipItem->IkUse)
		{
		case EIkUse::None:
			return FTransform::Identity;
			break;
		case EIkUse::Left:
			GetMesh()->TransformToBoneSpace(TEXT("hand_r"), SocketTransform.GetLocation(), SocketTransform.Rotator(), OutputPos, OutputRot);
			return FTransform(OutputRot, OutputPos, FVector(1, 1, 1));
			break;
		case EIkUse::Right:
			GetMesh()->TransformToBoneSpace(TEXT("hand_l"), SocketTransform.GetLocation(), SocketTransform.Rotator(), OutputPos, OutputRot);
			return FTransform(OutputRot, OutputPos, FVector(1, 1, 1));
			break;
		default:
			return FTransform::Identity;
			break;
		}

	}
	return FTransform::Identity;
}

void ABasePlayerCharacter::SetToolVisibility(ETool Tool, bool bIsVisibility)
{

	//FString CollisionString = GetName() + UEnum::GetValueAsString(Tool) + FString::Printf(TEXT("%s"), bIsVisibility ? TEXT("true") : TEXT("false"));
	//UKismetSystemLibrary::PrintString(this, CollisionString, true, true, FLinearColor(0.0f, 0.66f, 1.f, 1.0f), 3.f);

	switch (Tool)
	{
	case ETool::WoodAxe:
		WoodAxe->SetMeshVisibility(bIsVisibility);
		break;
	case ETool::PickAxe:
		PickAxe->SetMeshVisibility(bIsVisibility);
		break;
	case ETool::Sickle:
		Sickle->SetMeshVisibility(bIsVisibility);
		break;
	default:
		break;
	}
}

void ABasePlayerCharacter::MultiSetToolVisibility_Implementation(ETool Tool, bool bIsVisibility)
{
	SetToolVisibility(Tool, bIsVisibility);
}

void ABasePlayerCharacter::ServerSetToolCollisionEnable_Implementation(ETool Tool, bool bIsEnable)
{
	MultiSetToolCollisionEnable(Tool, bIsEnable);
}

void ABasePlayerCharacter::MultiSetToolCollisionEnable_Implementation(ETool Tool, bool bIsEnable)
{
	SetToolCollisionEnable(Tool, bIsEnable);
}

void ABasePlayerCharacter::SetToolCollisionEnable(ETool Tool, bool bIsEnable)
{


	if (HasAuthority()) {
		UKismetSystemLibrary::PrintString(this, TEXT("서버 에서 툴의 콜리전을 켜줌"));
		
		// Enum 값을 문자열로 변환
		FString EnumName = StaticEnum<ETool>()->GetNameStringByValue((int64)Tool);

		// 최종 메시지 작성
		FString Message = FString::Printf(TEXT("Enum: %s, Bool: %s"),
			*EnumName,
			bIsEnable ? TEXT("true") : TEXT("false"));

		UKismetSystemLibrary::PrintString(this, Message);

		switch (Tool)
		{
		case ETool::WoodAxe:
			WoodAxe->SetCollisionEnable(bIsEnable);
			break;
		case ETool::PickAxe:
			PickAxe->SetCollisionEnable(bIsEnable);
			break;
		case ETool::Sickle:
			Sickle->SetCollisionEnable(bIsEnable);
			break;
		default:
			break;
		}
	}
	else {
		UKismetSystemLibrary::PrintString(this, TEXT("멀티 에서 툴의 콜리전을 켜줌"));

		switch (Tool)
		{
		case ETool::WoodAxe:
			WoodAxe->SetCollisionEnable(bIsEnable);
			break;
		case ETool::PickAxe:
			PickAxe->SetCollisionEnable(bIsEnable);
			break;
		case ETool::Sickle:
			Sickle->SetCollisionEnable(bIsEnable);
			break;
		default:
			break;
		}
	}

	
}

bool ABasePlayerCharacter::GetIntractionActorCanNextActive()
{
	ADTS_GamePlayPlayerController* CurrentPlayerController = Cast<ADTS_GamePlayPlayerController>(GetController());	// 플레이어 컨트롤러로 변환

	if (!CurrentPlayerController || !CurrentPlayerController->InteractionActor) {
		return false;
	}

	IDTS_BPIInteraction* InterfaceInstance = Cast<IDTS_BPIInteraction>(CurrentPlayerController->InteractionActor);
	
	if (InterfaceInstance) {
		FString Message = FString::Printf(TEXT("상호작용 물체의 남은 HP : %d"), InterfaceInstance->Execute_GetInteractionHp(CurrentPlayerController->InteractionActor));
		UKismetSystemLibrary::PrintString(this, Message);
		if (CurrentPlayerController->InteractionActor) {
			if (InterfaceInstance->Execute_GetInteractionHp(CurrentPlayerController->InteractionActor) > 0) {
				// 지금 인터랙션 하고 있는 액터가 존재하고 HP가 0보다 클 때
				UKismetSystemLibrary::PrintString(this, TEXT("상호작용 실행 가능"));
				return true;
			}
			else {
				UKismetSystemLibrary::PrintString(this, TEXT("HP가 없음"));
			}
		}
		else {
			UKismetSystemLibrary::PrintString(this, TEXT("액터가 없음"));
		}
	}
	else {
		UKismetSystemLibrary::PrintString(this, TEXT("상호작용 인터페이스 없음"));
	}

	return false;
}

bool ABasePlayerCharacter::GetPickAxeHigh()
{

	return false;
}

bool ABasePlayerCharacter::GetPickAxeMiddle()
{
	return false;
}

void ABasePlayerCharacter::ServerDestory_Implementation(AActor* Actor)
{
	Actor->Destroy();
}

/// <summary>
///	변수를 리플리케이트 하기 위해 필요한 함수(참고하라고)
/// </summary>
void ABasePlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABasePlayerCharacter, bCanAttack);
	DOREPLIFETIME(ABasePlayerCharacter, ComboCount);
	DOREPLIFETIME(ABasePlayerCharacter, UpperBodyOverlay);
	DOREPLIFETIME(ABasePlayerCharacter, bIsAiming);
	DOREPLIFETIME(ABasePlayerCharacter, DrawAmount);
	DOREPLIFETIME(ABasePlayerCharacter, MoveX);
	DOREPLIFETIME(ABasePlayerCharacter, MoveY);
	DOREPLIFETIME(ABasePlayerCharacter, Yaw);
	DOREPLIFETIME(ABasePlayerCharacter, Pitch);
	DOREPLIFETIME(ABasePlayerCharacter, CurrentEquipItem);
	DOREPLIFETIME(ABasePlayerCharacter, IkUse);
}

void ABasePlayerCharacter::ServerSetYaw_Implementation(float NewYaw)
{
	Yaw = NewYaw;
}

void ABasePlayerCharacter::ServerSetPitch_Implementation(float NewPitch)
{
	Pitch = NewPitch;
}

void ABasePlayerCharacter::ServerSetMoveX_Implementation(float NewMoveX)
{
	MoveX = NewMoveX;
}

void ABasePlayerCharacter::ServerSetMoveY_Implementation(float NewMoveY)
{
	MoveY = NewMoveY;
}

void ABasePlayerCharacter::SetbOrientRotationToMovement(bool bOrientRotationToMovementValue)
{
	GetCharacterMovement()->bOrientRotationToMovement = bOrientRotationToMovementValue;
	bUseControllerRotationYaw = !bOrientRotationToMovementValue;
}

void ABasePlayerCharacter::ServerHit_Implementation(EDirection Direction)
{
	MultiHit(Direction);
}

void ABasePlayerCharacter::MultiHit_Implementation(EDirection Direction)
{
	FString Message;
	switch (Direction)
	{
	case EDirection::Forward:
		PlayMontageWithSession(HitMontage, HitSectionName[0]);
		Message = FString::Printf(TEXT("히트 몽타주 실행?"));
		UKismetSystemLibrary::PrintString(this, Message);
		break;
	case EDirection::Back:
		PlayMontageWithSession(HitMontage, HitSectionName[1]);
		break;
	case EDirection::Left:
		PlayMontageWithSession(HitMontage, HitSectionName[2]);
		break;
	case EDirection::Right:
		PlayMontageWithSession(HitMontage, HitSectionName[3]);
		break;
	default:
		PlayMontageWithSession(HitMontage, HitSectionName[0]);
		break;
	}
}


