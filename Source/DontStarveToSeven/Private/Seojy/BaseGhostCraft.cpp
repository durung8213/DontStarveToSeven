// Fill out your copyright notice in the Description page of Project Settings.


#include "Seojy/BaseGhostCraft.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"


// Sets default values
ABaseGhostCraft::ABaseGhostCraft()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ABaseGhostCraft::SwitchMaterialColor(int32 Number)
{
	UMaterialInterface* NewMaterial = nullptr;
	if (Number == 1)
	{
		//���������� ǥ��
		NewMaterial = MaterialRed;
	}
}

void ABaseGhostCraft::SetPreviewColor(UStaticMeshComponent* MeshCompo, int32 number)
{
	int32 NumSlots = MeshCompo->GetNumMaterials();
	FName ParameterName = FName(TEXT("BaseColor"));

	// ����ƽ �޽��� ������ ��� ���׸����� �ε�����ŭ �ݺ��� ����
	for (int SlotIndex = 0; SlotIndex < NumSlots; SlotIndex++)
	{
		// �ش� ������ ���׸��� �ν��Ͻ��� �̹� ���� �ν��Ͻ� ������ Ȯ��
		UMaterialInstanceDynamic* DynamicMatInstance = Cast<UMaterialInstanceDynamic>(MeshCompo->GetMaterial(SlotIndex));
		if (!DynamicMatInstance)
		{
			//���̳��� �ν��Ͻ��� NULL �� �ƴ϶��, �� ����� ���׸��� ���� ����ִٸ�.
			// �ش� �޽����� ���̳��� ���׸����� �����Ѵ�.
			DynamicMatInstance = MeshCompo->CreateDynamicMaterialInstance(SlotIndex, MeshCompo->GetMaterial(SlotIndex));

			if (!DynamicMatInstance)
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to create dynamic material instance"));
				continue;
			}
		}

		if (number == 1)
		{
			//true -> �ǹ��� ��ġ ������ ����
			DynamicMatInstance->SetVectorParameterValue(ParameterName, FLinearColor::Green);
		}
		else
		{
			// false -> �ǹ��� ��ġ �Ұ����� ����
			DynamicMatInstance->SetVectorParameterValue(ParameterName, FLinearColor::Red);
		}
	}

}

// Called when the game starts or when spawned
void ABaseGhostCraft::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseGhostCraft::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

