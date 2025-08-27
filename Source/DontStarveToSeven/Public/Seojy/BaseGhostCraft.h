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
	// �޽��� ������ �ִ� ��� ���׸��� ���� ������.
	//TArray<UMaterialInstance>* MeshMaterials;

public:

	//�ʷ� ���׸���
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Material")
	UMaterialInterface* MaterialGreen;

	//���� ���׸���
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Material")
	UMaterialInterface* MaterialRed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* ActorMeshCompo;

	// ���׸��� �ε���
	int32 CurrentMaterialIndex;

	// �ݸ��� ������
	float CollisionSize;
	

};
