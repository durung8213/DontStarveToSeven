// Fill out your copyright notice in the Description page of Project Settings.


#include "Hamkt/Actor/WeaponBase.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);

	//StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	//RootComponent = StaticMeshComp;
//	StaticMeshComp->AttachToComponent(RootComponent,)
	// **콜리전을 NoCollision**으로 설정
	//StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

