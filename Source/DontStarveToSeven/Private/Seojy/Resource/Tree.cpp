// Fill out your copyright notice in the Description page of Project Settings.


#include "Seojy/Resource/Tree.h"
#include "Kimmh/HUDnUI/UI/DTS_BaseInteractionUserWidget.h"

ATree::ATree()
{
	InteractionEnum = EInteraction::Tree;
	RequiredHits = 3;
	item.ItemName = "BP_Wood";
	item.ItemCount = 1;

	// ��ƼŬ �ϵ��ڵ�
	static ConstructorHelpers::FObjectFinder<UParticleSystem> Particle(
		TEXT("ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Destruction/P_Destruction_Wood.P_Destruction_Wood'")
	);
	if (Particle.Succeeded())
	{
		CollectPar = Particle.Object;
	}

	CollectParPath = FSoftObjectPath(TEXT("ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Destruction/P_Destruction_Wood.P_Destruction_Wood'"));

	ParticleOffset = FVector(0.0f, 0.0f, 150.0f);
}

ATree::~ATree()
{
}

void ATree::BeginPlay()
{
	Super::BeginPlay();

	// UI �̸� ����
	UDTS_BaseInteractionUserWidget* InteractionUserWidget = Cast<UDTS_BaseInteractionUserWidget>(InteractionWidget->GetWidget());
	if (InteractionUserWidget)
	{
		//������ ĳ��Ʈ �Ǹ�
		InteractionUserWidget->SetInteractionText(FText::FromString(TEXT("Tree")));
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
