// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ResourceItemStruct.generated.h"


/**
 * 
 */

 USTRUCT(Atomic, BlueprintType)
 struct DONTSTARVETOSEVEN_API FResourceItem : public FTableRowBase {
 	GENERATED_USTRUCT_BODY()
 
 public:
 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
 	UTexture2D* ResourceItemImage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ResourceItemName;
 
 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
 	int ResoucreItemCount;
 };

class DONTSTARVETOSEVEN_API ResourceItemStruct
{
public:
	ResourceItemStruct();
	~ResourceItemStruct();
};
