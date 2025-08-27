// Fill out your copyright notice in the Description page of Project Settings.


#include "TempLearn/Learn_DataTable.h"
#include "Engine/DataTable.h"
#include "Kimmh/Struct/CreaftingStruct.h"

/// <summary>
/// �����ڿ��� ������ ���̺� ��������
/// </summary>
Learn_DataTable::Learn_DataTable()
{
	// �����ڿ��� ����ü �����ͼ� �����ϴ��� ����ϱ�

	// Cpp ����
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTable1Object(TEXT("/Game/Kimmh/Blueprint/DT/DT_ItemsCreafting.DT_ItemsCreafting"/*���۷��� �ּ�*/));
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
/// ��Ÿ�ӿ��� ������ ���̺� ��������
/// </summary>
void Learn_DataTable::GetDataTable()
{
	// 1. ��Ÿ�� ���� ������ ���̺� ��������
	UDataTable* DataTable;		// ������ ���̺� ���� ����
	DataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Kimmh/Blueprint/DT/DT_ItemsCreafting.DT_ItemsCreafting"/*���۷��� �ּ�*/));	// ������ ���̺� �ҷ���
	if (DataTable) {
		// ������ ���̺��� �����ϸ�
		TArray<FName> RowNames;		// ��ü RowName �迭
		RowNames = DataTable->GetRowNames();	// ��� RowName �̾Ƴ���

		for (FName RowName : RowNames) {	// ��� ��� 1
			FCreafting*/* [F����ü��*] */ Row = DataTable->FindRow<FCreafting/* [F����ü��*] */>(RowName, TEXT("General"));
			Row->CreaftingImage/*����ü �� ����*/;	// ����ü �� ���� ��������
		}

		for (int i = 0; i < RowNames.Num(); i++) { // ��� ��� 2
			FCreafting*/* [F����ü��*] */ Row = DataTable->FindRow<FCreafting/* [F����ü��*] */>(RowNames[i], TEXT("General"));
			Row->CreaftingImage/*����ü �� ����*/;	// ����ü �� ���� ��������
		}
	}
	else {

	}
}
