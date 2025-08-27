// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kimmh/Interface/DTS_BPIInteraction.h"
#include "Base_InteractionActor.generated.h"

UCLASS()
class DONTSTARVETOSEVEN_API ABase_InteractionActor : public AActor, public IDTS_BPIInteraction
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABase_InteractionActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	/// <summary>
	/// 인터페이스 함수
	/// 위젯을 반환하는 함수이다
	/// </summary>
	/// <returns>상호작용 위젯의 주소</returns>
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GetWidget")
	UDTS_BaseInteractionUserWidget* GetBaseInteractionWidget();
	virtual UDTS_BaseInteractionUserWidget* GetBaseInteractionWidget_Implementation() override;

public:
	/// <summary>
	/// 위젯 컴포넌트
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Interaction")
	class UWidgetComponent* InteractionWidget;
};
