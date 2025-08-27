// Fill out your copyright notice in the Description page of Project Settings.
#include "Seojy/Craft/BonFire.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/Actor.h"
#include "Kimmh/HUDnUI/UI/DTS_BaseInteractionUserWidget.h"
#include "Leesh/SHTotalStruct.h"
#include "Leesh/SHInventoryComponent.h"
#include "Kismet/KismetSystemLibrary.h"

ABonFire::ABonFire()
{
	bReplicates = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// 스태틱 메시 초기화
	FireMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BonFire"));
	FireMesh->SetupAttachment(RootComponent);

	//트리거 초기화
	Trigger = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"));
	Trigger->InitSphereRadius(150.0f);
	Trigger->SetCollisionProfileName(TEXT("Trigger"));
	Trigger->SetupAttachment(RootComponent);
	
	// 파티클 초기화
	FireParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireParticle"));
	FireParticle->SetupAttachment(RootComponent);
	FireParticle->bAutoActivate = false;	

	// 실제 머테리얼 적용
	ActualMaterial = LoadObject<UMaterialInterface>(nullptr,
		TEXT("Material'/Game/Kimmh/Assets/SurvivalGameKitV1/Materials/M_EX_EnvAssets_Livables1.M_EX_EnvAssets_Livables1'"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshBase
	(TEXT("StaticMesh'/Game/Kimmh/Assets/SurvivalGameKitV1/Meshes/Static/SM_Campfire.SM_Campfire'"));
	if (MeshBase.Succeeded())
	{
		FireMesh->SetStaticMesh(MeshBase.Object);
		ActorMeshComponents.Add(FireMesh);
		UE_LOG(LogTemp, Warning, TEXT("Fire Mesh Success"));
	}

	//처음에 불과 상호작용 가능이 꺼진 상태

}

ABonFire::~ABonFire()
{
}

void ABonFire::SetCraftMode(ECraftMode NewMode)
{	
	ABaseCraft::SetCraftMode(NewMode);

	//트리거 이벤트 바인딩
	if (Trigger && CurrentMode == ECraftMode::Actual)
	{
		Trigger->OnComponentBeginOverlap.AddDynamic(this, &ABonFire::OnTriggerBegin);
		Trigger->OnComponentEndOverlap.AddDynamic(this, &ABonFire::OnTriggerEnd);
	}
}

void ABonFire::BeginPlay()
{
	Super::BeginPlay();

	// 모든 크래프팅 건물은 해당 함수를 초기화해야 된다.
	SetBuildingData(1, 1, ECraftType::BonFire);


	CraftName = "BP_BonFire";

	ActorMeshComponents.Add(FireMesh);
	InteractionEnum = EInteraction::Fire;


	// 위젯 컴포넌트에 있는 위젯이 내가 만든 위젯으로 케스트
	UDTS_BaseInteractionUserWidget* InteractionUserWidget = Cast<UDTS_BaseInteractionUserWidget>(InteractionWidget->GetWidget());
	if (InteractionUserWidget) {
		// 위젯이 케스트 되면
		InteractionUserWidget->SetInteractionText(FText::FromString(TEXT("Fire")));	// 위젯에 넣을 택스트 설정
		InteractionUserWidget->SetCollapsedWidget(); // 위젯이 안보이게 함
	}

	if (bIsFireOn)
	{
		FireParticle->Activate();
	}
}

void ABonFire::ServerTurnOnFire_Implementation()
{
	// 만약 불이 꺼져 있다면
	if (!bIsFireOn && bIsHaveWood)
	{
		// 인벤토리에서 장작 제거
		/*FInventory wood;
		wood.ItemName = "BP_Wood";
		wood.ItemCount = 1;
		PlayerPC->InventoryComponent->RemoveItem(wood,EInventoryType::Inventory);*/

		UKismetSystemLibrary::PrintString(this, TEXT("Server Fire On"));
		//다시 키도록 한다.
		bIsFireOn = true;
		FireParticle->Activate();
	}
}


// 트리거 오버랩 Start
void ABonFire::OnTriggerBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//플레이어만 체크 
	if (OtherActor && OtherActor->IsA(ACharacter::StaticClass()))
	{
		UKismetSystemLibrary::PrintString(this, TEXT("Player in BonFire"));
		//현재 범위 안에 들어와있음을 체크
		bPlayerInRange = true;

		// 플레이어 가져오기.
		ACharacter* Player = Cast<ACharacter>(OtherActor);
		PlayerPC = Cast<ADTS_GamePlayPlayerController>(Player->GetController());
		
		if (PlayerPC)
		{
			SetOwner(PlayerPC);          // ★ 소유권 이전 ★
		}

		// 인벤토리를 체크하는 타이머 실행
		GetWorldTimerManager().SetTimer(TimerHandle_CheckInventory, this,
			&ABonFire::CheckInventory, 1.0f, true);

		//UI 키기
	}
	else
	{
		UKismetSystemLibrary::PrintString(this, TEXT("No One in BonFire"));
	}
}

// 트리거 오버랩 End
void ABonFire::OnTriggerEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->IsA(ACharacter::StaticClass()))
	{
		bPlayerInRange = false;
		GetWorldTimerManager().ClearTimer(TimerHandle_CheckInventory);
		PlayerPC = nullptr;
	}
}

void ABonFire::Interaction_Implementation(ADTS_GamePlayPlayerController* InteractionPlayerController, ABasePlayerCharacter* InteractionPlayerCharacter)
{
	UKismetSystemLibrary::PrintString(this, TEXT("Interaction In"));
	// F 키를 눌렀을 때
	UE_LOG(LogTemp, Warning, TEXT("bIsFireOn : %d , bPlayerInRange : %d"), bIsFireOn, bPlayerInRange);

	if (!bIsFireOn && bPlayerInRange && bIsHaveWood)
	{
		//현재 불이 켜져 있지 않다면
		UKismetSystemLibrary::PrintString(this, TEXT("Fire On"));
		ServerTurnOnFire();
		return;
	}
	else if(bIsFireOn && bPlayerInRange)
		// 현재 불이 켜져 있다면
	{
		UKismetSystemLibrary::PrintString(this, TEXT("Pot On"));
		if (bIsFireOn && bPlayerInRange && !bIsHavePot)
		{
			//불은 켜져 있지만 솥은 없다면
			//솥이 필요하다는 알림 UI
			UKismetSystemLibrary::PrintString(this, TEXT("No Pot"));
		}
		else if (bIsFireOn && bPlayerInRange && bIsHavePot)
		{

			UKismetSystemLibrary::PrintString(this, TEXT("Pot Launch Start"));
			ServerPlaceCrockPot();
		}
	}
	else
	{
		UKismetSystemLibrary::PrintString(this, TEXT("No Interaction"));
	}

}

// 서버에서 솥 설치
void ABonFire::ServerPlaceCrockPot_Implementation()
{	
	if (!PlayerPC)
	{
		UKismetSystemLibrary::PrintString(this, TEXT("No PlayerPC"));
		return;
	}
	//인벤토리에서 솥 제거
	// 수정 요망
	//FInventory Pot;
	//Pot.ItemName = "CrockPot";
	//Pot.ItemCount = 1;
	//PlayerPC->InventoryComponent->RemoveItem(Pot, EInventoryType::Inventory);

	//불과 솥이 다 있을 때
	if (!CrockPotClass)
	{
		UKismetSystemLibrary::PrintString(this, TEXT("No Pot Class"));
		return;
	}

	FVector BonFireLocation = this->GetActorLocation();
	FVector SpawnLocation = BonFireLocation + FVector(0.f, 0.f, 25.0f);
	FRotator SpawnRotation = FRotator::ZeroRotator;
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	ACrockPot* Pot = GetWorld()->SpawnActor<ACrockPot>
		(
			CrockPotClass,
			SpawnLocation,
			SpawnRotation,
			SpawnParams
		);
	if (Pot)
	{
		// 모닥불에 붙이기.
		Pot->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	}

}

bool ABonFire::ServerPlaceCrockPot_Validate()
{
	return true;
}



void ABonFire::CheckInventory()
{
	FString pot = "BP_Pot";
	FString wood = "BP_Wood";

	UKismetSystemLibrary::PrintString(this, TEXT("CheckInventory"));

	// 불이 켜져있고, 플레이어가 유효할 때
	if (PlayerPC && bIsFireOn)
	{

		UKismetSystemLibrary::PrintString(this, TEXT("CheckInventory_If"));
		if (PlayerPC->InventoryComponent->SearchItemCount(pot, EInventoryType::Inventory) > 0)
		{
			//인벤토리 안에 해당되는 솥이 있을 때
			bIsHavePot = true;
			UKismetSystemLibrary::PrintString(this, TEXT("Pot Launch Start"));
			UE_LOG(LogTemp, Warning, TEXT("Player Have Pot"));
		}
		else
		{
			// 수정 요망
			bIsHavePot = true;
			UE_LOG(LogTemp, Warning, TEXT("Player No Pot"));
		}

	}
	// 불이 켜져 있지 않을 때
	else if (PlayerPC && !bIsFireOn)
	{
		if (PlayerPC->InventoryComponent->SearchItemCount(wood, EInventoryType::Inventory) > 0)
		{
			//인벤토리 안에 장작이 있을 때
			bIsHaveWood = true;
		}
		else
		{
			// 수정 요망
			bIsHaveWood = true;
		}
	}
}


void ABonFire::OnRep_FireOn()
{
	if (bIsFireOn)
	{
		FireParticle->Activate(true);
	}
	else
	{
		FireParticle->Deactivate();
	}

}


void ABonFire::Tick(float DeltaTime)
{
	if (Trigger)
	{
		// 디버그 삭제
		//FVector SphereLocation = Trigger->GetComponentLocation();
		//float SphereRadius = Trigger->GetScaledSphereRadius();
		//DrawDebugSphere(GetWorld(), SphereLocation, SphereRadius, 16, FColor::Green, false,
		//	1.0f, 0, 2.f);
	}
}

void ABonFire::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//현재 불 상태 복제
	DOREPLIFETIME(ABonFire, bIsFireOn);
	DOREPLIFETIME(ABonFire, bPlayerInRange);
	DOREPLIFETIME(ABonFire, bIsHavePot);
	DOREPLIFETIME(ABonFire, bIsHaveWood);
}
