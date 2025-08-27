// Fill out your copyright notice in the Description page of Project Settings.


#include "TempLearn/Learn_DataTable.h"
#include "Engine/DataTable.h"
#include "Kimmh/Struct/CreaftingStruct.h"

/// <summary>
/// 생성자에서 데이터 테이블 가져오기
/// </summary>
Learn_DataTable::Learn_DataTable()
{
	// 생성자에서 구조체 가져와서 저장하던가 사용하기

	// Cpp 파일
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTable1Object(TEXT("/Game/Kimmh/Blueprint/DT/DT_ItemsCreafting.DT_ItemsCreafting"/*레퍼런스 주소*/));
	if (DataTable1Object.Succeeded()) {
		DataTable1 = DataTable1Object.Object;
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("DataTable1Object Call Fail"));
	}
}

Learn_DataTable::~Learn_DataTable()
{

}

/// <summary>
/// 런타임에서 데이터 테이블 가져오기
/// </summary>
void Learn_DataTable::GetDataTable()
{
	// 1. 런타임 도중 데이터 테이블 가져오기
	UDataTable* DataTable;		// 데이터 테이블 변수 선언
	DataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Kimmh/Blueprint/DT/DT_ItemsCreafting.DT_ItemsCreafting"/*레퍼런스 주소*/));	// 데이터 테이블 불러옴
	if (DataTable) {
		// 데이터 테이블이 존재하면
		TArray<FName> RowNames;		// 전체 RowName 배열
		RowNames = DataTable->GetRowNames();	// 모든 RowName 뽑아내기

		for (FName RowName : RowNames) {	// 사용 방법 1
			FCreafting*/* [F구조체명*] */ Row = DataTable->FindRow<FCreafting/* [F구조체명*] */>(RowName, TEXT("General"));
			Row->CreaftingImage/*구조체 내 변수*/;	// 구조체 내 변수 가져오기
		}

		for (int i = 0; i < RowNames.Num(); i++) { // 사용 방법 2
			FCreafting*/* [F구조체명*] */ Row = DataTable->FindRow<FCreafting/* [F구조체명*] */>(RowNames[i], TEXT("General"));
			Row->CreaftingImage/*구조체 내 변수*/;	// 구조체 내 변수 가져오기
		}
	}
	else {

	}
}
