// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DTS_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class DONTSTARVETOSEVEN_API UDTS_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DataManage")
	FString NickName = TEXT("");

};
