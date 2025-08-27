// Fill out your copyright notice in the Description page of Project Settings.


#include "TempLearn/LearnServerMultiReplicated.h"
#include "Net/UnrealNetwork.h"	// 리플리케이트를 하기 위해서 반드시 추가해야 한다
#include "Kismet/KismetSystemLibrary.h"
// Sets default values
ALearnServerMultiReplicated::ALearnServerMultiReplicated()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

/// <summary>
///	변수를 리플리케이트 하기 위해 필요한 함수
/// 이 함수에 변수를 등록해야 리프리케이트가 정상적으로 작동한다
/// </summary>
void ALearnServerMultiReplicated::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ALearnServerMultiReplicated, MaxHp);
	DOREPLIFETIME(ALearnServerMultiReplicated, CurrentHp);
}

// 리플리케이트 노티파이 함수 선언, 변수의 리플리케이트가 일어날 때 클라이언트에서 실행된다
void ALearnServerMultiReplicated::OnRep_ChangeCurrentHp()
{
	FString Message = FString::Printf(TEXT("CurrentHp is Changed by Server then this Func Active in client"));
	UKismetSystemLibrary::PrintString(this, Message);
}

// 반드시 _Implementation을 붙여야 한다 에러떠도 컴파일은 정상적으로 되니 걱정 ㄴㄴ
void ALearnServerMultiReplicated::MultiFunc_Implementation()
{
	FString Message = FString::Printf(TEXT("Multicast Func Active"));
	UKismetSystemLibrary::PrintString(this, Message);
}

// 반드시 _Implementation을 붙여야 한다 에러떠도 컴파일은 정상적으로 되니 걱정 ㄴㄴ
void ALearnServerMultiReplicated::ServerFunc_Implementation()
{
	FString Message = FString::Printf(TEXT("Server Func Active"));
	UKismetSystemLibrary::PrintString(this, Message);
	MultiFunc();
	ClientFunc();
	CurrentHp = 100;
}

// 반드시 _Implementation을 붙여야 한다 에러떠도 컴파일은 정상적으로 되니 걱정 ㄴㄴ
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

