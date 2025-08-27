// Fill out your copyright notice in the Description page of Project Settings.


#include "Seojy/Resource/Rock.h"
#include "Kimmh/HUDnUI/UI/DTS_BaseInteractionUserWidget.h"

ARock::ARock()
{
	InteractionEnum = EInteraction::Rock;
	RequiredHits = 3;
	item.ItemName = "BP_Stone";
	item.ItemCount = 1;

	// ��ƼŬ �ϵ��ڵ�
	static ConstructorHelpers::FObjectFinder<UParticleSystem> Particle(
		TEXT("ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Destruction/P_Destruction_Concrete.P_Destruction_Concrete'")
	);
	if (Particle.Succeeded())
	{
		CollectPar = Particle.Object;
	}
	CollectParPath = FSoftObjectPath(TEXT("ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Destruction/P_Destruction_Concrete.P_Destruction_Concrete'"));

}

ARock::~ARock()
{
}

void ARock::BeginPlay()
{
	Super::BeginPlay();

	// UI �̸� ����
	UDTS_BaseInteractionUserWidget* InteractionUserWidget = Cast<UDTS_BaseInteractionUserWidget>(InteractionWidget->GetWidget());
	if (InteractionUserWidget)
	{
		//������ ĳ��Ʈ �Ǹ�
		InteractionUserWidget->SetInteractionText(FText::FromString(TEXT("Rock")));
		InteractionUserWidget->SetCollapsedWidget();
	}

	if (!CollectPar && HasAuthority())
	{
		CollectPar = LoadObject<UParticleSystem>(nullptr,
			TEXT("ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Destruction/P_Destruction_Concrete.P_Destruction_Concrete'"));
	}

	if (!CollectPar && CollectParPath.IsValid())
	{
		CollectPar = Cast<UParticleSystem>(CollectParPath.TryLoad());
	}

	ParticleOffset = FVector(0, 0, 40.0f);
	ParticleSize = FVector(0.8f);
}
