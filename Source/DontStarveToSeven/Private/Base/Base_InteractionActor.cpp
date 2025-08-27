// Fill out your copyright notice in the Description page of Project Settings.


#include "Base/Base_InteractionActor.h"
#include "Components/WidgetComponent.h"
#include "Kimmh/HUDnUI/UI/DTS_BaseInteractionUserWidget.h"

// Sets default values
ABase_InteractionActor::ABase_InteractionActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// ���� ������Ʈ ����
	InteractionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));

	// ��Ʈ ������Ʈ ���� (�̰��� ���� �޽� �Ǵ� ���̷�Ż �޽� ���� �־��ּ��� �˾Ƽ� �ְ� ��ġ �������ֱ�!)
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

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


}

// Called when the game starts or when spawned
void ABase_InteractionActor::BeginPlay()
{
	Super::BeginPlay();

	UDTS_BaseInteractionUserWidget* InteractionUserWidget = Cast<UDTS_BaseInteractionUserWidget>(InteractionWidget->GetWidget());		// ���� ������Ʈ�� �ִ� ������ ���� ���� �������� �ɽ�Ʈ
	if (InteractionUserWidget) {
		// ������ �ɽ�Ʈ �Ǹ�
		InteractionUserWidget->SetInteractionText(FText::FromString(TEXT("Rock")));	// ������ ���� �ý�Ʈ ����
		InteractionUserWidget->SetCollapsedWidget();		// ������ �Ⱥ��̰� ��
	}
}

UDTS_BaseInteractionUserWidget* ABase_InteractionActor::GetBaseInteractionWidget_Implementation()
{

	UDTS_BaseInteractionUserWidget* InteractionUserWidget = Cast<UDTS_BaseInteractionUserWidget>(InteractionWidget->GetWidget());
	if (InteractionUserWidget) {
		return InteractionUserWidget;
	}
	return nullptr;
}

