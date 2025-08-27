// Fill out your copyright notice in the Description page of Project Settings.


#include "Base/Base_InteractionActor.h"
#include "Components/WidgetComponent.h"
#include "Kimmh/HUDnUI/UI/DTS_BaseInteractionUserWidget.h"

// Sets default values
ABase_InteractionActor::ABase_InteractionActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// 위젯 컴포넌트 생성
	InteractionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));

	// 루트 컴포넌트 설정 (이것은 각자 메시 또는 스켈레탈 메시 위에 넣어주세요 알아서 넣고 위치 조정해주기!)
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

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


}

// Called when the game starts or when spawned
void ABase_InteractionActor::BeginPlay()
{
	Super::BeginPlay();

	UDTS_BaseInteractionUserWidget* InteractionUserWidget = Cast<UDTS_BaseInteractionUserWidget>(InteractionWidget->GetWidget());		// 위젯 컴포넌트에 있는 위젯이 내가 만든 위젯으로 케스트
	if (InteractionUserWidget) {
		// 위젯이 케스트 되면
		InteractionUserWidget->SetInteractionText(FText::FromString(TEXT("Rock")));	// 위젯에 넣을 택스트 설정
		InteractionUserWidget->SetCollapsedWidget();		// 위젯이 안보이게 함
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

