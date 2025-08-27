// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Leesh/SHTotalStruct.h"
#include "DTS_SaveGame.generated.h"

/**
 * 
 */
UCLASS()
class DONTSTARVETOSEVEN_API UDTS_SaveGame : public USaveGame
{
	GENERATED_BODY()
	

public:
	UPROPERTY(BlueprintReadWrite)
	FTotalData SavedTotalDataPack;

};
