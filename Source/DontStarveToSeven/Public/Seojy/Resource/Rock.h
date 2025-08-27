// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Seojy/Resource/Resource.h"
#include "Rock.generated.h"

/**
 * 
 */
UCLASS()
class DONTSTARVETOSEVEN_API ARock : public AResource
{
	GENERATED_BODY()

public:
	ARock();
	~ARock();

protected:
	virtual void BeginPlay() override;
};
