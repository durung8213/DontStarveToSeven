// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Seojy/BaseCraft.h"
#include "CrockPot.generated.h"

/**
 * 
 */
UCLASS()
class DONTSTARVETOSEVEN_API ACrockPot : public ABaseCraft
{
	GENERATED_BODY()
	
public:
	ACrockPot();
	~ACrockPot();

protected:
	virtual void BeginPlay() override;
};
