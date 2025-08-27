// Fill out your copyright notice in the Description page of Project Settings.


#include "Kimmh/Character/PlayerState/DTS_BasePlayerState.h"
//#include "DTS_BasePlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kimmh/HUDnUI/HUD/DTS_BaseGamePlayHUD.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kimmh/HUDnUI/UI/DTS_UserStateUserWidget.h"
#include "Leesh/SHTotalStruct.h"

ADTS_BasePlayerState::ADTS_BasePlayerState() {
	MaxHp = 100;
	CurrentHp = MaxHp;
	MaxMp = 100;
	CurrentMp = MaxMp;
	Atk = 10;
	Def = 0;
	Speed = 1;
	Critical = 0.3f;
	CriticalDamage = 2.0f;
	MaxHunger = 100;
	CurrentHunger = MaxHunger;
}

void ADTS_BasePlayerState::BeginPlay()
{
	/*if (GetPlayerController()) {
		if (GetPlayerController()->GetHUD()) {
			ADTS_BaseGamePlayHUD* CurrentHUD = Cast<ADTS_BaseGamePlayHUD>(GetPlayerController()->GetHUD());
			if (CurrentHUD) {
				if (CurrentHUD->DTSUserStateUserWidget) {
					CurrentHUD->DTSUserStateUserWidget->Init();
				}
				else {
					FString Message = FString::Printf(TEXT("DTSUserStateUserWidget is Null"));
					UKismetSystemLibrary::PrintString(this, Message, true, true, FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));
				}
			}
			else {
				FString Message = FString::Printf(TEXT("HUD Cast Fail"));
				UKismetSystemLibrary::PrintString(this, Message, true, true, FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));
			}
		}
	}*/
	
	
}

void ADTS_BasePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADTS_BasePlayerState, MaxHp);
	DOREPLIFETIME(ADTS_BasePlayerState, CurrentHp);
	DOREPLIFETIME(ADTS_BasePlayerState, MaxMp);
	DOREPLIFETIME(ADTS_BasePlayerState, CurrentMp);
	DOREPLIFETIME(ADTS_BasePlayerState, Atk);
	DOREPLIFETIME(ADTS_BasePlayerState, Def);
	DOREPLIFETIME(ADTS_BasePlayerState, Speed);
	DOREPLIFETIME(ADTS_BasePlayerState, Critical);
	DOREPLIFETIME(ADTS_BasePlayerState, CriticalDamage);
	DOREPLIFETIME(ADTS_BasePlayerState, MaxHunger);
	DOREPLIFETIME(ADTS_BasePlayerState, CurrentHunger);
}

// MaxHp를 반환하는 함수
float ADTS_BasePlayerState::GetMaxHp()
{
	return MaxHp;
}

// CurrentHp를 반환하는 함수
float ADTS_BasePlayerState::GetCurrentHp()
{
	return CurrentHp;
}

// MaxMp를 반환하는 함수
float ADTS_BasePlayerState::GetMaxMp()
{
	return MaxMp;
}

// CurrentMp를 반환하는 함수
float ADTS_BasePlayerState::GetCurrentMp()
{
	return CurrentMp;
}

float ADTS_BasePlayerState::GetAtk()
{
	return Atk;
}

float ADTS_BasePlayerState::GetDef()
{
	return Def;
}

float ADTS_BasePlayerState::GetSpeed()
{
	return Speed;
}

float ADTS_BasePlayerState::GetCritical()
{
	return Critical;
}

float ADTS_BasePlayerState::GetCriticalDamage()
{
	return CriticalDamage;
}

float ADTS_BasePlayerState::GetMaxHunger()
{
	return MaxHunger;
}

float ADTS_BasePlayerState::GetCurrentHunger()
{
	return CurrentHunger;
}



// 서버에서 MaxHp를 변경하는 함수
void ADTS_BasePlayerState::ServerSetMaxHp_Implementation(float NewMaxHp)
{
	MaxHp = NewMaxHp;
}

// 서버에서 MaxHp가 되면 리플리케이트 되는 함수
void ADTS_BasePlayerState::OnRep_ChangeMaxHp()
{
	GetUserStateUserWidget()->SetMaxHp(MaxHp);
}

// 서버에서 CurrentHp를 변경해 주는 함수
void ADTS_BasePlayerState::ServerSetCurrentHp_Implementation(float NewCurrentHp)
{
	CurrentHp = UKismetMathLibrary::FClamp(NewCurrentHp, 0, MaxHp);
}

// MaxHp의 리플리케이트 함수
void ADTS_BasePlayerState::OnRep_ChangeCurrentHp()
{
	// UI 변경
	GetUserStateUserWidget()->SetCurrentHp(CurrentHp);
}

// 서버에서 MaxMp를 변경해 주는 함수
void ADTS_BasePlayerState::ServerSetMaxMp_Implementation(float NewMaxMp)
{
	CurrentMp = UKismetMathLibrary::FClamp(NewMaxMp, 0, MaxMp);
}

void ADTS_BasePlayerState::OnRep_ChangeMaxMp()
{
	GetUserStateUserWidget()->SetMaxHp(MaxMp);
}

void ADTS_BasePlayerState::ServerSetCurrnetMp_Implementation(float NewCurrentMp)
{
}

void ADTS_BasePlayerState::OnRep_ChangeCurrentMp()
{
	GetUserStateUserWidget()->SetMaxHp(CurrentMp);
}

void ADTS_BasePlayerState::ServerSetAtk_Implementation(float NewAtk)
{
	Atk = NewAtk;
}

void ADTS_BasePlayerState::ServerSetDef_Implementation(float NewDef)
{
	Def = NewDef;
}

void ADTS_BasePlayerState::ServerSetSpeed_Implementation(float NewSpeed)
{
	Speed = NewSpeed;
}

void ADTS_BasePlayerState::ServerSetCritical_Implementation(float NewCritical)
{
	Critical = NewCritical;
}

void ADTS_BasePlayerState::ServerSetCriticalDamage_Implementation(float NewCriticalDamage)
{
	CriticalDamage = NewCriticalDamage;
}

void ADTS_BasePlayerState::ServerSetMaxHunger_Implementation(float NewMaxHunger)
{
	MaxHunger = NewMaxHunger;
}

void ADTS_BasePlayerState::OnRep_ChangeMaxHunger()
{
	GetUserStateUserWidget()->SetMaxHunger(MaxHunger);
}

void ADTS_BasePlayerState::ServerSetCurrentHunger_Implementation(float NewCurrentHunger)
{
	CurrentHunger = UKismetMathLibrary::FClamp(NewCurrentHunger, 0, MaxHunger);
}

void ADTS_BasePlayerState::OnRep_ChangeCurrentHunger()
{
	GetUserStateUserWidget()->SetCurrentHunger(CurrentHunger);
}

FCharacterStat ADTS_BasePlayerState::GetAllPlayerStat()
{
	return FCharacterStat(MaxHp,CurrentHp,MaxMp,CurrentMp, Atk,Def,Speed,Critical,CriticalDamage,MaxHunger,CurrentHunger);
	
}

void ADTS_BasePlayerState::SetAllPlayerStat(const FCharacterStat& InStat)
{
	MaxHp = InStat.MaxHP;
	CurrentHp = InStat.CurrentHP;
	MaxMp = InStat.MaxMp;
	CurrentMp = InStat.CurrentMp;
	Atk = InStat.Atk;
	Def = InStat.Def;
	Speed = InStat.Speed;
	Critical = InStat.Critical;
	CriticalDamage = InStat.CriticalDamage;
	MaxHunger = InStat.MaxHunger;
	CurrentHunger = InStat.CurrentHunger;
}

UDTS_UserStateUserWidget* ADTS_BasePlayerState::GetUserStateUserWidget()
{
	if (GetPlayerController()) {
		if (GetPlayerController()->GetHUD()) {
			ADTS_BaseGamePlayHUD* CurrentHUD = Cast<ADTS_BaseGamePlayHUD>(GetPlayerController()->GetHUD());
			if (CurrentHUD) {
				if (CurrentHUD->DTSUserStateUserWidget) {
					return CurrentHUD->DTSUserStateUserWidget;
				}
			}
		}
	}
	
	return nullptr;
}

void ADTS_BasePlayerState::ServerTakeDamage_Implementation(float Damage, AActor* DamageActor)
{
	float ClempedDamage = UKismetMathLibrary::FClamp((Damage - Def), 1, 999999999.0f);
	float NewCurrentHp = CurrentHp - ClempedDamage;
	ServerSetCurrentHp(NewCurrentHp);
}








