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

	// ���� ������Ʈ ����
	InteractionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));

	// ��Ʈ ������Ʈ ���� (�̰��� ���� �޽� �Ǵ� ���̷�Ż �޽� ���� �־��ּ��� �˾Ƽ� �ְ� ��ġ �������ֱ�!)
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));


	// �޽� ������Ʈ ��Ʈ�� �ֱ�.
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	InteractionWidget->SetupAttachment(RootComponent);

	// ���� Ŭ���� ���� (Blueprint���� �Ҵ� ����)
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Game/Kimmh/UI/Interaction/WBP_BaseInteratcion.WBP_BaseInteratcion_C"));
	if (WidgetClass.Succeeded())
	{
		InteractionWidget->SetWidgetClass(WidgetClass.Class);	// ���� ������Ʈ�� ���� Ŭ���� ����
	}

	InteractionWidget->SetDrawSize(FVector2D(100, 60)); // ���� ũ��
	InteractionWidget->SetWidgetSpace(EWidgetSpace::Screen); // ȭ�� �������� ����
	InteractionWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision); // �浹 ��Ȱ��ȭ



	// Ʈ���� ���� ����
	InteractionTrigger = CreateDefaultSubobject<UCapsuleComponent>(TEXT("InteractionTrigger"));
	InteractionTrigger->InitCapsuleSize(100.f, 300.f);
	InteractionTrigger->SetCollisionProfileName(TEXT("Trigger"));
	InteractionTrigger->bEditableWhenInherited = true;
	InteractionTrigger->SetupAttachment(RootComponent);

	//�̺�Ʈ ���ε�
	InteractionTrigger->OnComponentBeginOverlap.AddDynamic(this, &AResource::OnOverlap);
}


void AResource::BeginPlay()
{
	Super::BeginPlay();

	UDTS_BaseInteractionUserWidget* InteractionUserWidget = Cast<UDTS_BaseInteractionUserWidget>(InteractionWidget->GetWidget());		// ���� ������Ʈ�� �ִ� ������ ���� ���� �������� �ɽ�Ʈ
	if (InteractionUserWidget) {
		// ������ �ɽ�Ʈ �Ǹ�
		InteractionUserWidget->SetInteractionText(FText::FromString(TEXT("Object")));	// ������ ���� �ý�Ʈ ����
		InteractionUserWidget->SetCollapsedWidget();		// ������ �Ⱥ��̰� ��
	}
	//if (HasAuthority() && CollectPar)
	//{
	//	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CollectPar, GetActorLocation() + FVector(0, 0, 100));
	//}

		// ���������� ��ƼŬ ����. ������ �ɱ�!
	//if (HasAuthority() && CollectPar)
	//{
	//	FTimerHandle TimerHandle_DelayedParticle;
	//	GetWorld()->GetTimerManager().SetTimer(
	//		TimerHandle_DelayedParticle,
	//		this,
	//		&AResource::PlayInitialParticle,
	//		0.1f, // 0.1�� �� ����
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

// ���� �����ϱ�.
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

// ������ ���ҽ� ���Ϳ��� ������ �Ǿ��� ��
void AResource::OnOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABasePlayerCharacter* Player = Cast<ABasePlayerCharacter>(OtherActor->GetParentActor());

	if (!Player ||  RequiredHits <= 0)
		return;

	// Ŭ���̾�Ʈ �� ��
	if (!HasAuthority())
	{
		// �������� ���ͷ��� ��û
		UKismetSystemLibrary::PrintString(this, TEXT("Interaction Request to Server"));
		Server_HandleInteraction(SweepResult.ImpactPoint, Player);
	}
	else
	{
		// ������ ��, ������ ���� ����
		HandleInteractionInternal(SweepResult.ImpactPoint, Player);
		UKismetSystemLibrary::PrintString(this, TEXT("�������� ���� ����"));
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
	// �ش� ĳ������ ��Ʈ�ѷ��� ������
	ADTS_GamePlayPlayerController* PC = Cast<ADTS_GamePlayPlayerController>(Player->
		GetController());

	// �κ��丮�� ��ȿ�� ��, ������ �߰�
	if (PC && PC->InventoryComponent)
	{
		PC->InventoryComponent->AddItem(item, EInventoryType::Inventory);
	}
	// ��ƼŬ ����
	MulticastPlayParticleEffect(ImpactPoint);
	
	// �������� HP üũ
	Server_HandleHit();
}

// �ش� ������Ʈ�� ü�� ���� �̺�Ʈ�� �������� ó��.
void AResource::Server_HandleHit_Implementation()
{
	// ü�� ���� ó��
	RequiredHits = FMath::Clamp(RequiredHits - 1, 0, RequiredHits);

	// ü���� 0�� �ȴٸ�
	if (RequiredHits == 0)
	{

		// ���� ����
		UDTS_BaseInteractionUserWidget* InteractionUserWidget = Cast<UDTS_BaseInteractionUserWidget>(InteractionWidget->GetWidget());
		if (InteractionUserWidget) {
			InteractionUserWidget->SetCollapsedWidget();
		}

		//���� ����
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

// HP ��ȯ
int AResource::GetInteractionHp_Implementation()
{
	return RequiredHits;
}

// �ش� ���ͷ��� Enum ���� ��ȯ.
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