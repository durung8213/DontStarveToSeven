// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Seojy/Resource/Resource.h"
#include "ResourceSpawner.generated.h"

UCLASS()
class DONTSTARVETOSEVEN_API AResourceSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AResourceSpawner();

	UFUNCTION(BlueprintCallable, Category = "Spawner")
	void SpawnResource();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	FVector GetRandomLocationInBox() const;

	void GetLandScapeHeight(FVector& Location) const;

public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBoxComponent* SpawnArea;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AResource>> ResourceTypes;

	UPROPERTY(EditAnywhere)
	int32 SpawnCount = 10;
};
