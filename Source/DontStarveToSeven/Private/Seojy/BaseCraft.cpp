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

		// �� ���׸��� ���Կ� ���� ���� ���׸��� ����
		for (int32 SlotIndex = 0; SlotIndex < NumSlots; SlotIndex++)
		{
			// ���� Ghost ����� GhostMaterial�� ������� ���� �ν��Ͻ��� ����
			UMaterialInterface* BaseMaterial = nullptr;

			if (CurrentMode == ECraftMode::Ghost && GhostMaterial)
			{
				BaseMaterial = GhostMaterial;
			}
			else
			{
				BaseMaterial = ActorMeshComponents[0]->GetMaterial(SlotIndex);
			}

			// �޽� �迭�� ���׸��� �����ϱ�
			for (UStaticMeshComponent* mesh : ActorMeshComponents)
			{
				if (BaseMaterial)
				{
					UMaterialInstanceDynamic* DynMatInst = ActorMeshComponents[0]->CreateDynamicMaterialInstance(SlotIndex, BaseMaterial);
					if (DynMatInst)
					{
						// ĳ�� : �迭�� ����
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

	// ��Ʈ ���׸��� ����
	GhostMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/Game/Seojy/Material/M_GhostMaterial.M_GhostMaterial'"));

	// ���� ������Ʈ ����
	InteractionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
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


	//���� ���� Ȱ��ȭ
	bReplicates = true;

	// �±� �߰�
	Tags.Add(TEXT("Craft"));

	// ���ο� GUID ����
	CraftID = FGuid::NewGuid();

}
// HP ���� �Լ�

void ABaseCraft::BrokenCraft()
{
	// ��ƼŬ ����

	// ���� ��� �迭���� �ش� ���๰ �����ϱ�
	Destroy();
}

void ABaseCraft::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority()&& OtherActor && OtherActor->ActorHasTag(FName("Enemy")))
	{
		// ü�� �϶�
		TakeDamage(10.0f, OtherActor);


		// ��ƼŬ ����


	}
}

void ABaseCraft::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ABaseCraft::TakeDamage_Implementation(const float Amount, AActor* DamageActor)
{
	// ���� �ޱ�
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
	// ũ����Ʈ ���� ����Ʈ ����
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

	//ĳ���ص� �� ���� ���׸��� �ν��Ͻ��� ���� �Ķ���� ����
	for (UMaterialInstanceDynamic* DynMatInst : DynamicMatInstances)
	{
		if (DynMatInst)
		{
			// ���� ���׸��� �ش�Ǵ� �÷��� �Ķ���ͷ� ����.
			DynMatInst->SetVectorParameterValue(ParameterName, NewColor);
		}
	}

}

// �ǹ� ��� : �ٸ� ���� ����ġ.
void ABaseCraft::SetCraftMode(ECraftMode NewMode)
{
	CurrentMode = NewMode;
	ApplyBuildingMode();
}

// ũ����Ʈ HP ��������
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


// ���� ���� ��带 �����Ѵ�.
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
				//�� ������Ʈ�� 0�� ���Կ� ���� ���׸��� �ν��Ͻ� ����
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

	//������ ������Ƽ�� �����մϴ�.
	DOREPLIFETIME(ABaseCraft, CurrentMode);
	DOREPLIFETIME(ABaseCraft, BuildingData);
	DOREPLIFETIME(ABaseCraft, CraftHP);
}

