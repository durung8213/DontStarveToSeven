// Fill out your copyright notice in the Description page of Project Settings.


#include "Kimmh/Character/Controller/DTS_TitlePlayerController.h"
#include "Kismet/GameplayStatics.h"

void ADTS_TitlePlayerController::GoToServer(const FString& IpAddress)
{
	UGameplayStatics::OpenLevel(this, *IpAddress, true);

}
