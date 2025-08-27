// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Seojy/Resource/Resource.h"
#include "Bush.generated.h"

/**
 * 
 */
UCLASS()
class DONTSTARVETOSEVEN_API ABush : public AResource
{
	GENERATED_BODY()
	
public:
	ABush();
	~ABush();

protected:
	virtual void BeginPlay() override;
};
