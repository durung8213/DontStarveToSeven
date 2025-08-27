// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kimmh/Enum/IkUseEnum.h"
#include "Kimmh/Enum/UpperBodyOverlayEnum.h"
#include "Kimmh/Interface/DTS_BPIInteraction.h"
#include "SHBaseItem.generated.h"


UCLASS(Blueprintable)
class DONTSTARVETOSEVEN_API ASHBaseItem : public AActor, public IDTS_BPIInteraction
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASHBaseItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetItemMesh(UStaticMesh* NewMesh); // 아이템 스태틱 메시 변경 함수

	UFUNCTION(BlueprintCallable)
	FVector GetAttachLocation();

	UFUNCTION(BlueprintCallable)
	FRotator GetAttachRot();

	UFUNCTION(BlueprintCallable)
	FVector GetAnotherHandPos();

	UFUNCTION(BlueprintCallable)
	FRotator GetAnotherHandRot();

	UFUNCTION(BlueprintCallable)
	FTransform GetAnotherHandTransform();

	UFUNCTION(Server,Reliable ,BlueprintCallable)
	void ServerSetMeshSimulatePhysics(bool bSimulate);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void MulitSetMeshSimulatePhysics(bool bSimulate);

	void SetMeshSimulatePhysics(bool bSimulate);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerSetCollisionObjectTypeWeapon();

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void MulitSetCollisionObjectTypeWeapon();

	UFUNCTION()
	virtual void SetCollisionObjectTypeWeapon();

	/// <summary>
	/// 인터페이스 함수
	/// 위젯을 반환하는 함수이다
	/// </summary>
	/// <returns>상호작용 위젯의 주소</returns>
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GetWidget")
	UDTS_BaseInteractionUserWidget* GetBaseInteractionWidget();
	virtual UDTS_BaseInteractionUserWidget* GetBaseInteractionWidget_Implementation() override;

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class USphereComponent> Collision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EUpperOverlay UpperBodyOverlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName AttachSocket;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector AttachLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator AttachRot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EIkUse IkUse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector AnotherHandPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator AnotherHandRot;




public:
	/// <summary>
	/// 위젯 컴포넌트
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Interaction")
	class UWidgetComponent* InteractionWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ItemInteractionText = FText::FromString(TEXT(""));
};
