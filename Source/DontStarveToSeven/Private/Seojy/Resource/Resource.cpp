// Fill out your copyright notice in the Description page of Project Settings.


#include "Seojy/Resource/Resource.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kimmh/Character/Controller/DTS_GamePlayPlayerController.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Kimmh/BasePlayerCharacter.h"
#include "Components/WidgetComponent.h"
#include "Kimmh/HUDnUI/UI/DTS_BaseInteractionUserWidget.h"
#include "Leesh/SHInventoryComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UObject/SoftObjectPtr.h"
#include "Particles/ParticleSystem.h"
#include "FXSystem.h"

// Sets default values
AResource::AResource()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	SetReplicatingMovement(true);

	// 위젯 컴포넌트 생성
	InteractionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));

	// 루트 컴포넌트 설정 (이것은 각자 메시 또는 스켈레탈 메시 위에 넣어주세요 알아서 넣고 위치 조정해주기!)
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));


	// 메쉬 컴포넌트 루트에 넣기.
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	InteractionWidget->SetupAttachment(RootComponent);

	// 위젯 클래스 설정 (Blueprint에서 할당 가능)
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Game/Kimmh/UI/Interaction/WBP_BaseInteratcion.WBP_BaseInteratcion_C"));
	if (WidgetClass.Succeeded())
	{
		InteractionWidget->SetWidgetClass(WidgetClass.Class);	// 위젯 컴포넌트에 위젯 클래스 설정
	}

	InteractionWidget->SetDrawSize(FVector2D(100, 60)); // 위젯 크기
	InteractionWidget->SetWidgetSpace(EWidgetSpace::Screen); // 화면 공간으로 설정
	InteractionWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 충돌 비활성화



	// 트리거 영역 설정
	InteractionTrigger = CreateDefaultSubobject<UCapsuleComponent>(TEXT("InteractionTrigger"));
	InteractionTrigger->InitCapsuleSize(100.f, 300.f);
	InteractionTrigger->SetCollisionProfileName(TEXT("Trigger"));
	InteractionTrigger->bEditableWhenInherited = true;
	InteractionTrigger->SetupAttachment(RootComponent);

	//이벤트 바인딩
	InteractionTrigger->OnComponentBeginOverlap.AddDynamic(this, &AResource::OnOverlap);
}


void AResource::BeginPlay()
{
	Super::BeginPlay();

	UDTS_BaseInteractionUserWidget* InteractionUserWidget = Cast<UDTS_BaseInteractionUserWidget>(InteractionWidget->GetWidget());		// 위젯 컴포넌트에 있는 위젯이 내가 만든 위젯으로 케스트
	if (InteractionUserWidget) {
		// 위젯이 케스트 되면
		InteractionUserWidget->SetInteractionText(FText::FromString(TEXT("Object")));	// 위젯에 넣을 택스트 설정
		InteractionUserWidget->SetCollapsedWidget();		// 위젯이 안보이게 함
	}
	//if (HasAuthority() && CollectPar)
	//{
	//	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CollectPar, GetActorLocation() + FVector(0, 0, 100));
	//}

		// 서버에서만 파티클 생성. 딜레이 걸기!
	//if (HasAuthority() && CollectPar)
	//{
	//	FTimerHandle TimerHandle_DelayedParticle;
	//	GetWorld()->GetTimerManager().SetTimer(
	//		TimerHandle_DelayedParticle,
	//		this,
	//		&AResource::PlayInitialParticle,
	//		0.1f, // 0.1초 후 실행
	//		false
	//	);
	//}

}

void AResource::PlayInitialParticle()
{
	if (CollectPar)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CollectPar, GetActorLocation() + FVector(0, 0, 100));
	}
}

// 위젯 설정하기.
UDTS_BaseInteractionUserWidget* AResource::GetBaseInteractionWidget_Implementation()
{
	UDTS_BaseInteractionUserWidget* InteractionUserWidget = Cast<UDTS_BaseInteractionUserWidget>(InteractionWidget->GetWidget());
	if (InteractionUserWidget) {
		return InteractionUserWidget;
	}
	return nullptr;
}

// Called every frame
void AResource::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// 도구가 리소스 액터에게 오버랩 되었을 때
void AResource::OnOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABasePlayerCharacter* Player = Cast<ABasePlayerCharacter>(OtherActor->GetParentActor());

	if (!Player ||  RequiredHits <= 0)
		return;

	// 클라이언트 일 시
	if (!HasAuthority())
	{
		// 서버에게 인터랙션 요청
		UKismetSystemLibrary::PrintString(this, TEXT("Interaction Request to Server"));
		Server_HandleInteraction(SweepResult.ImpactPoint, Player);
	}
	else
	{
		// 서버일 시, 서버가 직접 진행
		HandleInteractionInternal(SweepResult.ImpactPoint, Player);
		UKismetSystemLibrary::PrintString(this, TEXT("서버에서 툴이 닿음"));
	}
}

void AResource::Server_HandleInteraction_Implementation(FVector ImpactPoint, ABasePlayerCharacter* Player)
{
	HandleInteractionInternal(ImpactPoint, Player);
}

void AResource::HandleInteractionInternal(FVector ImpactPoint, ABasePlayerCharacter* Player)
{
	if (!Player)
		return;
	// 해당 캐릭터의 컨트롤러를 가져옴
	ADTS_GamePlayPlayerController* PC = Cast<ADTS_GamePlayPlayerController>(Player->
		GetController());

	// 인벤토리가 유효할 시, 아이템 추가
	if (PC && PC->InventoryComponent)
	{
		PC->InventoryComponent->AddItem(item, EInventoryType::Inventory);
	}
	// 파티클 실행
	MulticastPlayParticleEffect(ImpactPoint);
	
	// 서버에서 HP 체크
	Server_HandleHit();
}

// 해당 오브젝트의 체력 감소 이벤트를 서버에서 처리.
void AResource::Server_HandleHit_Implementation()
{
	// 체력 감소 처리
	RequiredHits = FMath::Clamp(RequiredHits - 1, 0, RequiredHits);

	// 체력이 0이 된다면
	if (RequiredHits == 0)
	{

		// 위젯 삭제
		UDTS_BaseInteractionUserWidget* InteractionUserWidget = Cast<UDTS_BaseInteractionUserWidget>(InteractionWidget->GetWidget());
		if (InteractionUserWidget) {
			InteractionUserWidget->SetCollapsedWidget();
		}

		//액터 삭제
		Destroy();
		UE_LOG(LogTemp, Warning, TEXT("Destroy Tree"));
	}
}

void AResource::MulticastPlayParticleEffect_Implementation(FVector Location)
{
	UKismetSystemLibrary::PrintString(this, TEXT("Multi Particle Play"));
	if (!CollectPar && CollectParPath.IsValid())
	{
		CollectPar = Cast<UParticleSystem>(CollectParPath.TryLoad());
	}

	if (!CollectPar)
	{
		UE_LOG(LogTemp, Warning, TEXT("Client %s: particle load fail"), *GetName());
		return;
	}

	FVector AdjustedLocation = GetActorLocation() + ParticleOffset;

	UParticleSystemComponent* PSC = UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(),
		CollectPar,
		AdjustedLocation,
		FRotator::ZeroRotator,
		ParticleSize
	);
	//DrawDebugSphere(GetWorld(), AdjustedLocation, 50.f, 12, FColor::Red, false, 2.f);

}

// HP 반환
int AResource::GetInteractionHp_Implementation()
{
	return RequiredHits;
}

// 해당 인터랙션 Enum 변수 반환.
EInteraction AResource::GetInteractionType_Implementation()
{
	return InteractionEnum;
}

void AResource::MulticastPlayDestroyEffect_Implementation()
{
	if (DestroyPar)
	{
		FVector AdjustedLocation = GetActorLocation() + ParticleOffset;

		UParticleSystemComponent* PSC = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			DestroyPar,
			AdjustedLocation
		);
		//DrawDebugSphere(GetWorld(), AdjustedLocation, 50.f, 12, FColor::Red, false, 2.f);

	}
}

void AResource::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AResource, RequiredHits);
}