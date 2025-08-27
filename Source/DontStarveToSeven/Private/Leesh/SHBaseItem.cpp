// Fill out your copyright notice in the Description page of Project Settings.


#include "Leesh/SHBaseItem.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kimmh/HUDnUI/UI/DTS_BaseInteractionUserWidget.h"


// Sets default values
ASHBaseItem::ASHBaseItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetupAttachment(RootComponent);;

	Mesh->SetSimulatePhysics(true);

	// 메시 파일 경로를 찾아서 설정
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Kimmh/Assets/Animation/SpearAndHalberdAnimset/Models/Mesh/Spear_Spear_LOD0.Spear_Spear_LOD0"));
	if (MeshAsset.Succeeded())
	{
		Mesh->SetStaticMesh(MeshAsset.Object);
	}

	ItemName = "";
	ItemCount = 1;

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

	Collision->SetCollisionProfileName(TEXT("Custom"));
	Collision->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel4); // 예: Interaction

	bReplicates = true;
}

// Called when the game starts or when spawned
void ASHBaseItem::BeginPlay()
{
	Super::BeginPlay();
	Mesh->SetSimulatePhysics(true);
	UDTS_BaseInteractionUserWidget* InteractionUserWidget = Cast<UDTS_BaseInteractionUserWidget>(InteractionWidget->GetWidget());		// 위젯 컴포넌트에 있는 위젯이 내가 만든 위젯으로 케스트
	if (InteractionUserWidget) {
		// 위젯이 케스트 되면
		InteractionUserWidget->SetInteractionText(ItemInteractionText);	// 위젯에 넣을 택스트 설정
		InteractionUserWidget->SetCollapsedWidget();		// 위젯이 안보이게 함
	}

	
}

// Called every frame
void ASHBaseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void ASHBaseItem::SetItemMesh(UStaticMesh* NewMesh)
{
	if (Mesh && NewMesh)
	{
		Mesh->SetStaticMesh(NewMesh);
	}
}


FVector ASHBaseItem::GetAttachLocation()
{
	return FVector(AttachLocation);
}

FRotator ASHBaseItem::GetAttachRot()
{
	return FRotator(AttachRot);
}

FVector ASHBaseItem::GetAnotherHandPos()
{
	if (Mesh->DoesSocketExist(TEXT("AnotherHandSocket")))
	{
		FTransform SocketTransform = Mesh->GetSocketTransform(TEXT("AnotherHandSocket"), RTS_World);
		return SocketTransform.GetLocation();
	}

	return FVector(AnotherHandPos);
}

FRotator ASHBaseItem::GetAnotherHandRot()
{
	if (Mesh->DoesSocketExist(TEXT("AnotherHandSocket")))
	{
		FTransform SocketTransform = Mesh->GetSocketTransform(TEXT("AnotherHandSocket"), RTS_World);
		return SocketTransform.Rotator();
	}
	return FRotator(AnotherHandRot);
}

FTransform ASHBaseItem::GetAnotherHandTransform()
{
	if (Mesh->DoesSocketExist(TEXT("AnotherHandSocket")))
	{
		return Mesh->GetSocketTransform(TEXT("AnotherHandSocket"), RTS_World);
	}
	return FTransform::Identity;
}


void ASHBaseItem::ServerSetCollisionObjectTypeWeapon_Implementation()
{
	MulitSetCollisionObjectTypeWeapon();
}

void ASHBaseItem::MulitSetCollisionObjectTypeWeapon_Implementation()
{
	SetCollisionObjectTypeWeapon();
}

void ASHBaseItem::SetCollisionObjectTypeWeapon()
{
	Collision->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel7);
	Mesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel7);
}

void ASHBaseItem::ServerSetMeshSimulatePhysics_Implementation(bool bSimulate)
{
	MulitSetMeshSimulatePhysics(bSimulate);
}

void ASHBaseItem::MulitSetMeshSimulatePhysics_Implementation(bool bSimulate)
{
	SetMeshSimulatePhysics(bSimulate);
}

void ASHBaseItem::SetMeshSimulatePhysics(bool bSimulate)
{
	Mesh->SetSimulatePhysics(bSimulate);
}


UDTS_BaseInteractionUserWidget* ASHBaseItem::GetBaseInteractionWidget_Implementation()
{
	UDTS_BaseInteractionUserWidget* InteractionUserWidget = Cast<UDTS_BaseInteractionUserWidget>(InteractionWidget->GetWidget());
	if (InteractionUserWidget) {
		return InteractionUserWidget;
	}
	return nullptr;
}



