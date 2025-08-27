// Fill out your copyright notice in the Description page of Project Settings.


#include "TempLearn/LearnServerMultiReplicated.h"
#include "Net/UnrealNetwork.h"	// ���ø�����Ʈ�� �ϱ� ���ؼ� �ݵ�� �߰��ؾ� �Ѵ�
#include "Kismet/KismetSystemLibrary.h"
// Sets default values
ALearnServerMultiReplicated::ALearnServerMultiReplicated()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

/// <summary>
///	������ ���ø�����Ʈ �ϱ� ���� �ʿ��� �Լ�
/// �� �Լ��� ������ ����ؾ� ����������Ʈ�� ���������� �۵��Ѵ�
/// </summary>
void ALearnServerMultiReplicated::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ALearnServerMultiReplicated, MaxHp);
	DOREPLIFETIME(ALearnServerMultiReplicated, CurrentHp);
}

// ���ø�����Ʈ ��Ƽ���� �Լ� ����, ������ ���ø�����Ʈ�� �Ͼ �� Ŭ���̾�Ʈ���� ����ȴ�
void ALearnServerMultiReplicated::OnRep_ChangeCurrentHp()
{
	FString Message = FString::Printf(TEXT("CurrentHp is Changed by Server then this Func Active in client"));
	UKismetSystemLibrary::PrintString(this, Message);
}

// �ݵ�� _Implementation�� �ٿ��� �Ѵ� �������� �������� ���������� �Ǵ� ���� ����
void ALearnServerMultiReplicated::MultiFunc_Implementation()
{
	FString Message = FString::Printf(TEXT("Multicast Func Active"));
	UKismetSystemLibrary::PrintString(this, Message);
}

// �ݵ�� _Implementation�� �ٿ��� �Ѵ� �������� �������� ���������� �Ǵ� ���� ����
void ALearnServerMultiReplicated::ServerFunc_Implementation()
{
	FString Message = FString::Printf(TEXT("Server Func Active"));
	UKismetSystemLibrary::PrintString(this, Message);
	MultiFunc();
	ClientFunc();
	CurrentHp = 100;
}

// �ݵ�� _Implementation�� �ٿ��� �Ѵ� �������� �������� ���������� �Ǵ� ���� ����
void ALearnServerMultiReplicated::ClientFunc_Implementation()
{
	FString Message = FString::Printf(TEXT("Client Func Active"));
	UKismetSystemLibrary::PrintString(this, Message);
}

// Called when the game starts or when spawned
void ALearnServerMultiReplicated::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle TestTimer;

	GetWorld()->GetTimerManager().SetTimer(
		TestTimer,
		this,
		&ALearnServerMultiReplicated::ServerFunc,
		5.f,
		false
	);
}

// Called every frame
void ALearnServerMultiReplicated::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

