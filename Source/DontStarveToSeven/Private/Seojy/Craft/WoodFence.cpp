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

	// 메시 생성 및 루트 설정
	//RootComponent = FenceMesh;
	FenceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = FenceMesh;


	//Damage Zone 생성 및 루트에 부착

	DamageZone = CreateDefaultSubobject<UBoxComponent>(TEXT("DamageZone"));
	DamageZone->SetupAttachment(RootComponent);

	

	// 콜리전 설정 : Enemy 하고만 Overlap이 발생하도록 
	DamageZone->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DamageZone->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	DamageZone->SetCollisionResponseToAllChannels(ECR_Ignore);
	DamageZone->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	// 원본 머테리얼 저장
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
	
		// 위젯 컴포넌트에 있는 위젯이 내가 만든 위젯으로 케스트
	UDTS_BaseInteractionUserWidget* InteractionUserWidget = Cast<UDTS_BaseInteractionUserWidget>(InteractionWidget->GetWidget());
	if (InteractionUserWidget) {
		// 위젯이 케스트 되면
		InteractionUserWidget->SetInteractionText(FText::FromString(TEXT("Fire")));	// 위젯에 넣을 택스트 설정
		InteractionUserWidget->SetCollapsedWidget(); // 위젯이 안보이게 함
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

	// 위젯 컴포넌트에 있는 위젯이 내가 만든 위젯으로 케스트
	UDTS_BaseInteractionUserWidget* InteractionUserWidget = Cast<UDTS_BaseInteractionUserWidget>(InteractionWidget->GetWidget());
	if (InteractionUserWidget) {
		// 위젯이 케스트 되면
		InteractionUserWidget->SetInteractionText(FText::FromString(TEXT("Fire")));	// 위젯에 넣을 택스트 설정
		InteractionUserWidget->SetCollapsedWidget(); // 위젯이 안보이게 함
	}
}


void AWoodFence::Server_ApplyDamageToEnemy_Implementation(AActor* EnemyActor)
{
	if (!HasAuthority())
	{
		UE_LOG(LogTemp, Log, TEXT("No Server"));
		return;
	}


	// 서버에서만 데미지 처리.
	if (EnemyActor && IsValid(EnemyActor))
	{
		UE_LOG(LogTemp, Log, TEXT("Damaged"));
		// ApplyDamage를 통해 피해를 적용
		UGameplayStatics::ApplyDamage(EnemyActor, Damage, 
			GetInstigatorController(), this, UDamageType::StaticClass());

	}
}

// 적에게 공격하는 오버랩
void AWoodFence::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && OtherActor->ActorHasTag(FName("Enemy")))
	{
		//적 태그가 붙은 액터에 대해서만 타이머를 시작해 데미지를 적용
		UE_LOG(LogTemp, Log, TEXT("Overlap"));
		// 해당 액터에 붙은 타이머가 없다면 타이머 시작.
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

// 적에게 공격하는 오버랩 (End)
void AWoodFence::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// 적 태그가 붙은 액터가 영역을 벗어나면 타이머 종료
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
		FVector BoxExtent = DamageZone->GetScaledBoxExtent(); // BoxExtent는 로컬 스페이스니까 꼭 스케일 반영해야 함
		FRotator BoxRotation = DamageZone->GetComponentRotation();

		//DrawDebugBox(
		//	GetWorld(),
		//	BoxLocation,
		//	BoxExtent,
		//	BoxRotation.Quaternion(),
		//	FColor::Red,
		//	true,         // 지속 시간 이후 사라짐
		//	0.1f,          // 지속 시간
		//	0,             // 디버그 우선순위
		//	2.0f           // 선 두께
		//);
	}
}
