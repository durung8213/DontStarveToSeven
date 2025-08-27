// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Hamkt/Enemy/AI/BPI_EnemyAI.h"
#include "EnemyHealthBar.generated.h"

/**
 * 
 */
UCLASS()
class DONTSTARVETOSEVEN_API UEnemyHealthBar : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UI")
	float GetPercent() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	TScriptInterface<IBPI_EnemyAI> HealthOwner;
	
};
