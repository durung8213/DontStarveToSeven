// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Leesh/SHTotalStruct.h"
#include "Leesh/SHInventoryComponent.h"
#include "SHPlayerController.generated.h"

/**
 * 
 */




UCLASS()
class DONTSTARVETOSEVEN_API ASHPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ASHPlayerController();


protected:
	virtual void BeginPlay() override;  // 게임 시작 시 실행

public:


	// 입력 바인딩 함수
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void SetupInputComponent() override;

	// 입력 처리 함수
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "Input")
	void OnOneKeyPressed();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	USHInventoryComponent* InventoryComponent;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "Inventory")
	FString NickName = TEXT("");
	

	// 서버(게임모드) → 클라이언트: 닉네임 요청
	UFUNCTION(Client, Reliable, BlueprintCallable)
	void ClientRequestNickname();

	UFUNCTION(BlueprintCallable)
	void GetNickName();

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "DataManage")
	void ServerSetNickName(const FString& NewNickName);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "DataManage")
	void ServerTossInfoToGameMode();


	


};
