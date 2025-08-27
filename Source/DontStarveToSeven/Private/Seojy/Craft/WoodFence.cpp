// Fill out your copyright notice in the Description page of Project Settings.


#include "Seojy/Craft/WoodFence.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"
#include "Kimmh/HUDnUI/UI/DTS_BaseInteractionUserWidget.h"


AWoodFence::AWoodFence()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	// �޽� ���� �� ��Ʈ ����
	//RootComponent = FenceMesh;
	FenceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = FenceMesh;


	//Damage Zone ���� �� ��Ʈ�� ����

	DamageZone = CreateDefaultSubobject<UBoxComponent>(TEXT("DamageZone"));
	DamageZone->SetupAttachment(RootComponent);

	

	// �ݸ��� ���� : Enemy �ϰ� Overlap�� �߻��ϵ��� 
	DamageZone->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DamageZone->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	DamageZone->SetCollisionResponseToAllChannels(ECR_Ignore);
	DamageZone->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	// ���� ���׸��� ����
	ActualMaterial = LoadObject<UMaterialInterface>(nullptr,
		TEXT("/Game/Seojy/Fab/WesternDesertTown/Material/MI_TrimWood.MI_TrimWood"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshBase
	(TEXT("StaticMesh'/Game/Seojy/Fab/WesternDesertTown/Mesh/Wood/SM_Firewood_Stand.SM_Firewood_Stand'"));
	if (MeshBase.Succeeded())
	{
		FenceMesh->SetStaticMesh(MeshBase.Object);
		ActorMeshComponents.Add(FenceMesh);
		UE_LOG(LogTemp, Warning, TEXT("FenceMesh Success"));
	}
	
	DamageInterval = 0.5f;
	Damage = 5.0f;
	
		// ���� ������Ʈ�� �ִ� ������ ���� ���� �������� �ɽ�Ʈ
	UDTS_BaseInteractionUserWidget* InteractionUserWidget = Cast<UDTS_BaseInteractionUserWidget>(InteractionWidget->GetWidget());
	if (InteractionUserWidget) {
		// ������ �ɽ�Ʈ �Ǹ�
		InteractionUserWidget->SetInteractionText(FText::FromString(TEXT("Fire")));	// ������ ���� �ý�Ʈ ����
		InteractionUserWidget->SetCollapsedWidget(); // ������ �Ⱥ��̰� ��
	}
}

AWoodFence::~AWoodFence()
{
}


void AWoodFence::BeginPlay()
{
	Super::BeginPlay();

	SetBuildingData(1, 1, ECraftType::WoodFence);

	CraftName = "BP_WoodFence";

	// ���� ������Ʈ�� �ִ� ������ ���� ���� �������� �ɽ�Ʈ
	UDTS_BaseInteractionUserWidget* InteractionUserWidget = Cast<UDTS_BaseInteractionUserWidget>(InteractionWidget->GetWidget());
	if (InteractionUserWidget) {
		// ������ �ɽ�Ʈ �Ǹ�
		InteractionUserWidget->SetInteractionText(FText::FromString(TEXT("Fire")));	// ������ ���� �ý�Ʈ ����
		InteractionUserWidget->SetCollapsedWidget(); // ������ �Ⱥ��̰� ��
	}
}


void AWoodFence::Server_ApplyDamageToEnemy_Implementation(AActor* EnemyActor)
{
	if (!HasAuthority())
	{
		UE_LOG(LogTemp, Log, TEXT("No Server"));
		return;
	}


	// ���������� ������ ó��.
	if (EnemyActor && IsValid(EnemyActor))
	{
		UE_LOG(LogTemp, Log, TEXT("Damaged"));
		// ApplyDamage�� ���� ���ظ� ����
		UGameplayStatics::ApplyDamage(EnemyActor, Damage, 
			GetInstigatorController(), this, UDamageType::StaticClass());

	}
}

// ������ �����ϴ� ������
void AWoodFence::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && OtherActor->ActorHasTag(FName("Enemy")))
	{
		//�� �±װ� ���� ���Ϳ� ���ؼ��� Ÿ�̸Ӹ� ������ �������� ����
		UE_LOG(LogTemp, Log, TEXT("Overlap"));
		// �ش� ���Ϳ� ���� Ÿ�̸Ӱ� ���ٸ� Ÿ�̸� ����.
		if (!EnemyDamageTimers.Contains(OtherActor))
		{
			FTimerHandle TimerHandle;
			FTimerDelegate TimerDel;
			TimerDel.BindUFunction(this, FName("Server_ApplyDamageToEnemy"), OtherActor);
			GetWorldTimerManager().SetTimer(TimerHandle, TimerDel, DamageInterval, true);
			EnemyDamageTimers.Add(OtherActor, TimerHandle);
		}
	}
}

// ������ �����ϴ� ������ (End)
void AWoodFence::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// �� �±װ� ���� ���Ͱ� ������ ����� Ÿ�̸� ����
	if (OtherActor && OtherActor != this && OtherActor->ActorHasTag(FName("Enemy")))
	{
		if (EnemyDamageTimers.Contains(OtherActor))
		{
			FTimerHandle TimerHandle = EnemyDamageTimers[OtherActor];
			GetWorldTimerManager().ClearTimer(TimerHandle);
			EnemyDamageTimers.Remove(OtherActor);
		}
	}
}

void AWoodFence::SetCraftMode(ECraftMode NewMode)
{
	Super::SetCraftMode(NewMode);

	if (DamageZone && CurrentMode == ECraftMode::Actual)
	{
		UE_LOG(LogTemp, Warning, TEXT("WoodFence Binding Success"));
		DamageZone->OnComponentBeginOverlap.AddDynamic(this, &AWoodFence::OnOverlapBegin);
		DamageZone->OnComponentEndOverlap.AddDynamic(this, &AWoodFence::OnOverlapEnd);
	}

}

void AWoodFence::Tick(float DeltaTime)
{
	if (DamageZone)
	{
		FVector BoxLocation = DamageZone->GetComponentLocation();
		FVector BoxExtent = DamageZone->GetScaledBoxExtent(); // BoxExtent�� ���� �����̽��ϱ� �� ������ �ݿ��ؾ� ��
		FRotator BoxRotation = DamageZone->GetComponentRotation();

		//DrawDebugBox(
		//	GetWorld(),
		//	BoxLocation,
		//	BoxExtent,
		//	BoxRotation.Quaternion(),
		//	FColor::Red,
		//	true,         // ���� �ð� ���� �����
		//	0.1f,          // ���� �ð�
		//	0,             // ����� �켱����
		//	2.0f           // �� �β�
		//);
	}
}
