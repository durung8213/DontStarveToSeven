// Fill out your copyright notice in the Description page of Project Settings.


#include "Seojy/Craft/CrockPot.h"

ACrockPot::ACrockPot()
{
	bReplicates = true;
	CurrentMode = ECraftMode::Actual;
}

ACrockPot::~ACrockPot()
{
}

void ACrockPot::BeginPlay()
{
	CraftName = "BP_CrockPot";
}
