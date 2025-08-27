// Fill out your copyright notice in the Description page of Project Settings.


#include "Hamkt/Actor/BossResetLocation.h"

// Sets default values
ABossResetLocation::ABossResetLocation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABossResetLocation::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABossResetLocation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

