// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseGhostCraft.generated.h"

UCLASS()
class DONTSTARVETOSEVEN_API ABaseGhostCraft : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseGhostCraft();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SwitchMaterialColor(int32 Number);

	UFUNCTION(BlueprintCallable, Category = "Material")
	void SetPreviewColor(UStaticMeshComponent* MeshCompo, int32 number);



protected:
	virtual void BeginPlay() override;
	
private:
	// 메쉬가 가지고 있는 모든 머테리얼에 대한 정보들.
	//TArray<UMaterialInstance>* MeshMaterials;

public:

	//초록 머테리얼
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Material")
	UMaterialInterface* MaterialGreen;

	//빨강 머테리얼
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Material")
	UMaterialInterface* MaterialRed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* ActorMeshCompo;

	// 머테리얼 인덱스
	int32 CurrentMaterialIndex;

	// 콜리전 사이즈
	float CollisionSize;
	

};
