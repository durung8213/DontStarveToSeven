// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Learn_DataTable.generated.h"
/**
 * 
 */
USTRUCT(Atomic, BlueprintType)
struct DONTSTARVETOSEVEN_API FTest : public FTableRowBase {
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CreaftingType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CreaftingTime;
};

class DONTSTARVETOSEVEN_API Learn_DataTable
{
public:
	

	Learn_DataTable();
	~Learn_DataTable();

	void GetDataTable();

public:
	// 헤더 (.h) 파일
	UDataTable* DataTable1;
};
