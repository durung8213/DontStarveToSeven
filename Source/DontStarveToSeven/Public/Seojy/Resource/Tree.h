// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Seojy/Resource/Resource.h"
#include "Tree.generated.h"

/**
 * 
 */
UCLASS()
class DONTSTARVETOSEVEN_API ATree : public AResource
{
	GENERATED_BODY()
	
public:
	ATree();
	~ATree();

protected:
	virtual void BeginPlay() override;
};
