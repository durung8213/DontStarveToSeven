// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kimmh/Character/Controller/BasePlayerController.h"
#include "DTS_TitlePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DONTSTARVETOSEVEN_API ADTS_TitlePlayerController : public ABasePlayerController
{
	GENERATED_BODY()
	
public:
	void GoToServer(const FString& IpAddress);
};
