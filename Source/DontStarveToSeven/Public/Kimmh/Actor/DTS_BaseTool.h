// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DTS_BaseTool.generated.h"

UCLASS()
class DONTSTARVETOSEVEN_API ADTS_BaseTool : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADTS_BaseTool();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnCapsuleBeginOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void SetCollisionEnable(bool bIsEnable);

	UFUNCTION(Server, Reliable)
	void ServerCollisionEnable(bool bIsEnable);

	UFUNCTION()
	void SetMeshVisibility(bool bIsVisibility);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* ToolMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* DetectedComponent;
};
