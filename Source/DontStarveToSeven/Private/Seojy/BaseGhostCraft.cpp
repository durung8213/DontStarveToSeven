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
		//빨강색으로 표시
		NewMaterial = MaterialRed;
	}
}

void ABaseGhostCraft::SetPreviewColor(UStaticMeshComponent* MeshCompo, int32 number)
{
	int32 NumSlots = MeshCompo->GetNumMaterials();
	FName ParameterName = FName(TEXT("BaseColor"));

	// 스태틱 메쉬에 설정된 모든 머테리얼의 인덱스만큼 반복문 실행
	for (int SlotIndex = 0; SlotIndex < NumSlots; SlotIndex++)
	{
		// 해당 슬롯의 머테리얼 인스턴스가 이미 동적 인스턴스 인지를 확인
		UMaterialInstanceDynamic* DynamicMatInstance = Cast<UMaterialInstanceDynamic>(MeshCompo->GetMaterial(SlotIndex));
		if (!DynamicMatInstance)
		{
			//다이나믹 인스턴스가 NULL 이 아니라면, 즉 제대로 머테리얼 값이 들어있다면.
			// 해당 메쉬에서 다이나믹 머테리얼을 생성한다.
			DynamicMatInstance = MeshCompo->CreateDynamicMaterialInstance(SlotIndex, MeshCompo->GetMaterial(SlotIndex));

			if (!DynamicMatInstance)
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to create dynamic material instance"));
				continue;
			}
		}

		if (number == 1)
		{
			//true -> 건물이 설치 가능한 상태
			DynamicMatInstance->SetVectorParameterValue(ParameterName, FLinearColor::Green);
		}
		else
		{
			// false -> 건물이 설치 불가능한 상태
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

