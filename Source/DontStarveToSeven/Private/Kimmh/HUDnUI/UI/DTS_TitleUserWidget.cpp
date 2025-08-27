// Fill out your copyright notice in the Description page of Project Settings.


#include "Kimmh/HUDnUI/UI/DTS_TitleUserWidget.h"
#include "Components/Button.h"
#include "Components/ScaleBox.h"
#include "Components/EditableTextBox.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Kimmh/HUDnUI/UI/DTS_BasePopUpUserWidget.h"
#include "Kimmh/HUDnUI/UI/DTS_ExitGameUpUserWidget.h"
#include "Kimmh/Character/Controller/DTS_TitlePlayerController.h"
#include "Leesh/GameInstance/DTS_GameInstance.h"

void UDTS_TitleUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	NewGameButton->OnClicked.AddDynamic(this, &UDTS_TitleUserWidget::OnNewGameButtonClick);
	NewGameButton->OnHovered.AddDynamic(this, &UDTS_TitleUserWidget::OnNewGameButtonHovered);
	NewGameButton->OnUnhovered.AddDynamic(this, &UDTS_TitleUserWidget::OnNewGameButtonUnHovered);

	GameStartButton->OnClicked.AddDynamic(this, &UDTS_TitleUserWidget::OnGameStartButtonClick);
	GameStartButton->OnHovered.AddDynamic(this, &UDTS_TitleUserWidget::OnGameStartButtonHovered);
	GameStartButton->OnUnhovered.AddDynamic(this, &UDTS_TitleUserWidget::OnGameStartButtonUnHovered);

	WithFriendButton->OnClicked.AddDynamic(this, &UDTS_TitleUserWidget::OnWithFriendButtonClick);
	WithFriendButton->OnHovered.AddDynamic(this, &UDTS_TitleUserWidget::OnWithFriendButtonHovered);
	WithFriendButton->OnUnhovered.AddDynamic(this, &UDTS_TitleUserWidget::OnWithFriendButtonUnHovered);

	SettingButton->OnClicked.AddDynamic(this, &UDTS_TitleUserWidget::OnSettingButtonClick);
	SettingButton->OnHovered.AddDynamic(this, &UDTS_TitleUserWidget::OnSettingButtonHovered);
	SettingButton->OnUnhovered.AddDynamic(this, &UDTS_TitleUserWidget::OnSettingButtonUnHovered);

	ExitButton->OnClicked.AddDynamic(this, &UDTS_TitleUserWidget::OnExitButtonClick);
	ExitButton->OnHovered.AddDynamic(this, &UDTS_TitleUserWidget::OnExitButtonHovered);
	ExitButton->OnUnhovered.AddDynamic(this, &UDTS_TitleUserWidget::OnExitButtonUnHovered);

	WBP_BasePopUp->SetVisibility(ESlateVisibility::Collapsed);
	WBP_PopUpExitGame->SetVisibility(ESlateVisibility::Collapsed);
}

void UDTS_TitleUserWidget::OnNewGameButtonClick()
{
	if (IsNicknameViolation(NickNameEditableTextBox->GetText())) {
		// �г��� �˻��ؼ� ���� �г����̹Ƿ� ���� ���� �Ѿ��
		FString ServerPath = FPaths::Combine(FPaths::ProjectDir(), TEXT("Binaries/Win64/MyGameServer.exe"));
		FString Args = TEXT("-log"); // �ʿ�� ���̳� ��Ʈ ���� �߰�
		FPlatformProcess::CreateProc(*ServerPath, *Args, true, false, false, nullptr, 0, nullptr, nullptr);
	}
	else {
		// �г��� �˻翡�� ������ �г����̹Ƿ� �˾� ����
		ShowNicknameViolationPopUp();
	}
}

void UDTS_TitleUserWidget::OnNewGameButtonHovered()
{
	NewGameButtonScaleBox->SetRenderScale(FVector2D(1.2f, 1.2f));
}

void UDTS_TitleUserWidget::OnNewGameButtonUnHovered()
{
	NewGameButtonScaleBox->SetRenderScale(FVector2D(1.f, 1.f));
}

void UDTS_TitleUserWidget::OnGameStartButtonClick()
{
	if (IsNicknameViolation(NickNameEditableTextBox->GetText())) {
		// �г��� �˻��ؼ� ���� �г����̹Ƿ� ���� ���� �Ѿ��
		
	}
	else {
		// �г��� �˻翡�� ������ �г����̹Ƿ� �˾� ����
		ShowNicknameViolationPopUp();
	}
}

void UDTS_TitleUserWidget::OnGameStartButtonHovered()
{
	GameStartButtonScaleBox->SetRenderScale(FVector2D(1.2f, 1.2f));
}

void UDTS_TitleUserWidget::OnGameStartButtonUnHovered()
{
	GameStartButtonScaleBox->SetRenderScale(FVector2D(1.f, 1.f));
}

void UDTS_TitleUserWidget::OnWithFriendButtonClick()
{
	if (IsNicknameViolation(NickNameEditableTextBox->GetText())) {
		// �г��� �˻��ؼ� ���� �г���

		UDTS_GameInstance* CurrentInstance = Cast<UDTS_GameInstance>(GetGameInstance());

		if (CurrentInstance) {
			CurrentInstance->NickName = NickNameEditableTextBox->GetText().ToString();
		}

		ADTS_TitlePlayerController* CurrentController = Cast<ADTS_TitlePlayerController>(GetOwningPlayer());
		if (CurrentController) {
			CurrentController->GoToServer(IPAddressEditableTextBox->GetText().ToString());
		}
	}
	else {
		// �г��� �˻翡�� ������ �г����̹Ƿ� �˾� ����
		ShowNicknameViolationPopUp();
	}
}

void UDTS_TitleUserWidget::OnWithFriendButtonHovered()
{
	WithFriendButtonScaleBox->SetRenderScale(FVector2D(1.2f, 1.2f));
}

void UDTS_TitleUserWidget::OnWithFriendButtonUnHovered()
{
	WithFriendButtonScaleBox->SetRenderScale(FVector2D(1.f, 1.f));
}

void UDTS_TitleUserWidget::OnSettingButtonClick()
{
}

void UDTS_TitleUserWidget::OnSettingButtonHovered()
{
	ScaleBoxSettingButton->SetRenderScale(FVector2D(1.2f, 1.2f));
}

void UDTS_TitleUserWidget::OnSettingButtonUnHovered()
{
	ScaleBoxSettingButton->SetRenderScale(FVector2D(1.f, 1.f));
}

void UDTS_TitleUserWidget::OnExitButtonClick()
{
	WBP_PopUpExitGame->SetVisibility(ESlateVisibility::Visible);
}

void UDTS_TitleUserWidget::OnExitButtonHovered()
{
	ExitButtonScaleBox->SetRenderScale(FVector2D(1.2f, 1.2f));
}

void UDTS_TitleUserWidget::OnExitButtonUnHovered()
{
	ExitButtonScaleBox->SetRenderScale(FVector2D(1.f, 1.f));
}

bool UDTS_TitleUserWidget::IsNicknameViolation(const FText& NicknameText)
{
	FString Nickname = NicknameText.ToString();

	// 1. ���� �˻�
	if (Nickname.Len() > 12)
	{
		return false;
	}

	// 2. ��� ���ԵǾ����� (�ҹ���/�빮��)
	const FRegexPattern AlphaPattern(TEXT("^[A-Za-z]+$"));
	FRegexMatcher AlphaMatcher(AlphaPattern, Nickname);

	if (!AlphaMatcher.FindNext())
	{
		return false;
	}

	// 3. Ư������ ���� ���� (������ false)
	const FRegexPattern SpecialCharPattern(TEXT("[^A-Za-z0-9]")); // ���ĺ�+���� ������ ����
	FRegexMatcher SpecialCharMatcher(SpecialCharPattern, Nickname);

	if (SpecialCharMatcher.FindNext())
	{
		return false;
	}

	return true;
}

bool UDTS_TitleUserWidget::IsServerReachable(const FString& AddressWithPort)
{
	FString IP;
	FString PortStr;

	if (!AddressWithPort.Split(TEXT(":"), &IP, &PortStr))
	{
		// IP�� ��Ʈ�� �Ҹ����� ������ �˾� ����
		UE_LOG(LogTemp, Error, TEXT("Invalid address format. Use IP:Port"));
		return false;
	}

	int32 Port = FCString::Atoi(*PortStr);

	ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
	TSharedRef<FInternetAddr> Addr = SocketSubsystem->CreateInternetAddr();

	bool bIsValid;
	Addr->SetIp(*IP, bIsValid);
	Addr->SetPort(Port);

	if (!bIsValid)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid IP address: %s"), *IP);
		return false;
	}

	FSocket* Socket = SocketSubsystem->CreateSocket(NAME_Stream, TEXT("PingSocket"), false);
	bool bCanConnect = Socket->Connect(*Addr);

	Socket->Close();
	SocketSubsystem->DestroySocket(Socket);

	return bCanConnect;
}

void UDTS_TitleUserWidget::ShowNicknameViolationPopUp()
{
	WBP_BasePopUp->SetVisibility(ESlateVisibility::Visible);
	WBP_BasePopUp->SetPopUpMessage(FText::FromString(TEXT("NickError")));
}

void UDTS_TitleUserWidget::ShowServerReachablePopUp()
{
	WBP_BasePopUp->SetVisibility(ESlateVisibility::Visible);
	WBP_BasePopUp->SetPopUpMessage(FText::FromString(TEXT("Server connection failed")));
}
