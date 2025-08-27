// Fill out your copyright notice in the Description page of Project Settings.


#include "Kimmh/Actor/DTS_BaseTool.h"
#include "Components/MeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Seojy/Resource/Resource.h"
#include "Kimmh/BasePlayerCharacter.h"
#include "Kimmh/Character/Controller/DTS_GamePlayPlayerController.h"

// Sets default values
ADTS_BaseTool::ADTS_BaseTool()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ToolMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ToolMesh"));

	RootComponent = ToolMesh;

	DetectedComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionCapsule"));
	DetectedComponent->SetupAttachment(RootComponent);


	
}

// Called when the game starts or when spawned
void ADTS_BaseTool::BeginPlay()
{
	Super::BeginPlay();
	DetectedComponent->OnComponentBeginOverlap.AddDynamic(this, &ADTS_BaseTool::OnCapsuleBeginOverlap);
	DetectedComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetMeshVisibility(false);
}

// Called every frame
void ADTS_BaseTool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//FString CollisionString = GetName() + UEnum::GetValueAsString(DetectedComponent->GetCollisionEnabled());
	//UKismetSystemLibrary::PrintString(this, CollisionString, true, true, FLinearColor(0.0f, 0.66f, 1.f, 1.0f), 0.0f);


}

void ADTS_BaseTool::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/*ECollisionChannel Channel = OverlappedComp->GetCollisionObjectType();
	FString DebugText = FString::Printf(TEXT("Collision Channel: %d"), (int32)Channel);
	UKismetSystemLibrary::PrintString(this, DebugText);*/
	if (!HasAuthority()) {
		AResource* CurrentResource = Cast<AResource>(OtherActor);
		if (CurrentResource) {
			DetectedComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			UKismetSystemLibrary::PrintString(this, TEXT("Tool Overlap Resource"));

			ABasePlayerCharacter* CurrentToolParent = Cast<ABasePlayerCharacter>(GetParentActor());

			if (CurrentToolParent) {
				ADTS_GamePlayPlayerController* CurrentToolParentPlayerController = Cast< ADTS_GamePlayPlayerController>(CurrentToolParent->GetController());
				if (CurrentToolParentPlayerController) {
					if (IDTS_BPIInteraction* Interface = Cast<IDTS_BPIInteraction>(CurrentResource))
					{
						IDTS_BPIInteraction::Execute_Interaction(CurrentResource, CurrentToolParentPlayerController, CurrentToolParent);
					}
				}
			}

		}
	}
	

}

void ADTS_BaseTool::SetCollisionEnable(bool bIsEnable)
{
	ServerCollisionEnable(bIsEnable);
	if (bIsEnable) {
		DetectedComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	else {
		DetectedComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ADTS_BaseTool::ServerCollisionEnable_Implementation(bool bIsEnable)
{
	if (bIsEnable) {
		DetectedComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	else {
		DetectedComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ADTS_BaseTool::SetMeshVisibility(bool bIsVisibility)
{
	ToolMesh->SetVisibility(bIsVisibility);
}

