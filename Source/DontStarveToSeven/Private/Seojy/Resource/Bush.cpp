// Fill out your copyright notice in the Description page of Project Settings.


#include "Seojy/Resource/Bush.h"
#include "Kimmh/HUDnUI/UI/DTS_BaseInteractionUserWidget.h"

ABush::ABush()
{
	InteractionEnum = EInteraction::Bush;
	RequiredHits = 1;	
	item.ItemName = "BP_Grass";
	item.ItemCount = 1;

	// 파티클 하드코딩
	static ConstructorHelpers::FObjectFinder<UParticleSystem> Particle(
		TEXT("ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Hit/P_Vegetation.P_Vegetation'")
	);
	if (Particle.Succeeded())
	{
		CollectPar = Particle.Object;
	}

	CollectParPath = FSoftObjectPath(TEXT("ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Hit/P_Vegetation.P_Vegetation'"));

}

ABush::~ABush()
{
}

void ABush::BeginPlay()
{
	Super::BeginPlay();
	// UI 이름 설정
	UDTS_BaseInteractionUserWidget* InteractionUserWidget = Cast<UDTS_BaseInteractionUserWidget>(InteractionWidget->GetWidget());
	if (InteractionUserWidget)
	{
		//위젯이 캐스트 되면
		InteractionUserWidget->SetInteractionText(FText::FromString(TEXT("Bush")));
		InteractionUserWidget->SetCollapsedWidget();
	}

	if (!CollectPar && HasAuthority())
	{
		CollectPar = LoadObject<UParticleSystem>(nullptr,
			TEXT("/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Destruction/P_Destruction_Wood.P_Destruction_Wood"));
	}

	if (!CollectPar && CollectParPath.IsValid())
	{
		CollectPar = Cast<UParticleSystem>(CollectParPath.TryLoad());
	}

}
