// Fill out your copyright notice in the Description page of Project Settings.


#include "Seojy/BaseCraft.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kimmh/HUDnUI/UI/DTS_BaseInteractionUserWidget.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
//#include "Hamkt/Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"

void ABaseCraft::BeginPlay()
{
	Super::BeginPlay();


	if (ActorMeshComponents.Num() != 0)
	{
		int32 NumSlots = ActorMeshComponents[0]->GetNumMaterials();

		// 각 머테리얼 슬롯에 대한 동적 머테리얼 생성
		for (int32 SlotIndex = 0; SlotIndex < NumSlots; SlotIndex++)
		{
			// 만약 Ghost 모드라면 GhostMaterial를 기반으로 동적 인스턴스를 생성
			UMaterialInterface* BaseMaterial = nullptr;

			if (CurrentMode == ECraftMode::Ghost && GhostMaterial)
			{
				BaseMaterial = GhostMaterial;
			}
			else
			{
				BaseMaterial = ActorMeshComponents[0]->GetMaterial(SlotIndex);
			}

			// 메쉬 배열에 머테리얼 적용하기
			for (UStaticMeshComponent* mesh : ActorMeshComponents)
			{
				if (BaseMaterial)
				{
					UMaterialInstanceDynamic* DynMatInst = ActorMeshComponents[0]->CreateDynamicMaterialInstance(SlotIndex, BaseMaterial);
					if (DynMatInst)
					{
						// 캐싱 : 배열에 저장
						DynamicMatInstances.Add(DynMatInst);
						mesh->SetMaterial(SlotIndex, DynMatInst);
					}
				}
			}
		}
	}

	ApplyBuildingMode();


}
// Sets default values
ABaseCraft::ABaseCraft()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add(TEXT("Craft"));

	// 고스트 머테리얼 설정
	GhostMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/Game/Seojy/Material/M_GhostMaterial.M_GhostMaterial'"));

	// 위젯 컴포넌트 생성
	InteractionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
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


	//액터 복제 활성화
	bReplicates = true;

	// 태그 추가
	Tags.Add(TEXT("Craft"));

	// 새로운 GUID 생성
	CraftID = FGuid::NewGuid();

}
// HP 증감 함수

void ABaseCraft::BrokenCraft()
{
	// 파티클 실행

	// 게임 모드 배열에서 해당 건축물 제거하기
	Destroy();
}

void ABaseCraft::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority()&& OtherActor && OtherActor->ActorHasTag(FName("Enemy")))
	{
		// 체력 하락
		TakeDamage(10.0f, OtherActor);


		// 파티클 실행


	}
}

void ABaseCraft::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ABaseCraft::TakeDamage_Implementation(const float Amount, AActor* DamageActor)
{
	// 공격 받기
	ApplyDamage(false, Amount);

	//
}

UDTS_BaseInteractionUserWidget* ABaseCraft::GetBaseInteractionWidget_Implementation()
{
	UDTS_BaseInteractionUserWidget* InteractionUserWidget = Cast<UDTS_BaseInteractionUserWidget>(InteractionWidget->GetWidget());
	if (InteractionUserWidget) {
		return InteractionUserWidget;
	}
	return nullptr;
}

EInteraction ABaseCraft::GetInteractionType_Implementation()
{
	return InteractionEnum;
}

void ABaseCraft::OnRep_CraftHP()
{
	if (CraftHP <= 0)
	{
		BrokenCraft();
	}
}

void ABaseCraft::Multi_PlayHitEffect_Implementation(const FVector& SpawnLocation)
{
	if (HitEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
			HitEffect,
			this->GetActorTransform());
	}
	
}


void ABaseCraft::Multi_PlayDestroyEffect_Implementation()
{
	// 크래프트 삭제 이펙트 실행
	if (DestroyEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
			DestroyEffect,
			this->GetActorTransform());
	}
}



void ABaseCraft::SetPreviewColor(bool bIsCanPlace)
{
	bIsCanBuild = bIsCanPlace;
	FName ParameterName(TEXT("DefaultColor"));
	FLinearColor NewColor = bIsCanPlace ? FLinearColor::Green : FLinearColor::Red;
	//UE_LOG(LogTemp, Warning, TEXT("bMyBool: %d, Setting Color : %s"), bIsCanPlace,*NewColor.ToString());

	//캐싱해둔 각 동적 머테리얼 인스턴스에 대해 파라미터 적용
	for (UMaterialInstanceDynamic* DynMatInst : DynamicMatInstances)
	{
		if (DynMatInst)
		{
			// 동적 머테리얼에 해당되는 컬러를 파라미터로 적용.
			DynMatInst->SetVectorParameterValue(ParameterName, NewColor);
		}
	}

}

// 건물 모드 : 다른 모드로 스위치.
void ABaseCraft::SetCraftMode(ECraftMode NewMode)
{
	CurrentMode = NewMode;
	ApplyBuildingMode();
}

// 크래프트 HP 가져오기
float ABaseCraft::GetCraftHP() const
{
	return CraftHP;
}

void ABaseCraft::ApplyDamage_Implementation(bool IsPlus, float amount)
{
	if (!HasAuthority())
		return;

	if (IsPlus)
	{
		CraftHP = FMath::Clamp(CraftHP + amount, 0.0f, 100.0f);
	}
	else
	{
		CraftHP = FMath::Clamp(CraftHP - amount, 0.0f, 100.0f);
	}

	if (CraftHP <= 0.0f)
	{
		BrokenCraft();
	}
}


// 현재 건축 모드를 세팅한다.
void ABaseCraft::ApplyBuildingMode()
{
	if (ActorMeshComponents.Num() ==0)
		return;

	DynamicMatInstances.Empty();

	for (int32 i = 0; i < ActorMeshComponents.Num(); i++)
	{
		UStaticMeshComponent* MeshComp = ActorMeshComponents[i];
		if (!MeshComp)
		{
			continue;
		}

		if (CurrentMode == ECraftMode::Ghost)
		{
			if (GhostMaterial)
			{
				//각 컴포넌트의 0번 슬롯에 동적 머테리얼 인스턴스 생성
				UMaterialInstanceDynamic* DynMatInst = MeshComp->CreateDynamicMaterialInstance(0, GhostMaterial);
				if (DynMatInst)
				{
					DynamicMatInstances.Add(DynMatInst);
					MeshComp->SetMaterial(0, DynMatInst);
					UE_LOG(LogTemp, Log, TEXT("AddBuildingMode : Created dynamic Material"));
				}
				MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}
		}
		else if (CurrentMode == ECraftMode::Actual)
		{
			if (ActualMaterial)
			{
				MeshComp->SetMaterial(0, ActualMaterial);
			}
			MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}
	}
	
}

void ABaseCraft::SetBuildingData(int32 Width, int32 Height, ECraftType CraftType)
{
	BuildingData.Width = Width;
	BuildingData.Height = Height;
	BuildingData.BuildingType = CraftType;
}

// Called every frame
void ABaseCraft::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCraft::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//복제할 프로퍼티를 지정합니다.
	DOREPLIFETIME(ABaseCraft, CurrentMode);
	DOREPLIFETIME(ABaseCraft, BuildingData);
	DOREPLIFETIME(ABaseCraft, CraftHP);
}

