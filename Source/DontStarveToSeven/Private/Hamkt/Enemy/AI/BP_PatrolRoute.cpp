// Fill out your copyright notice in the Description page of Project Settings.


#include "Hamkt/Enemy/AI/BP_PatrolRoute.h"

// Sets default values
ABP_PatrolRoute::ABP_PatrolRoute()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABP_PatrolRoute::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABP_PatrolRoute::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

