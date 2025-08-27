// Fill out your copyright notice in the Description page of Project Settings.


#include "Kimmh/Camera/BaseCamera.h"
#include "Camera/CameraComponent.h"

// Sets default values
ABaseCamera::ABaseCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABaseCamera::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

