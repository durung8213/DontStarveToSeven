// Fill out your copyright notice in the Description page of Project Settings.


#include "Hamkt/Actor/BossAttackZoneBase.h"
#include "GameFramework/Character.h"
#include "Hamkt/Enemy/EnemyBoss/EnemyBossBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"

// Sets default values
ABossAttackZoneBase::ABossAttackZoneBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	Tags.Add(TEXT("Zone"));

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	RootComponent = CollisionSphere;
	CollisionSphere->SetGenerateOverlapEvents(true);

	// Overlap 이벤트 바인딩
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ABossAttackZoneBase::OnOverlapBegin);
	CollisionSphere->OnComponentEndOverlap.AddDynamic(this, &ABossAttackZoneBase::OnOverlapEnd);

}

void ABossAttackZoneBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		ServerBeginOverlap(OtherActor);
	}
}

void ABossAttackZoneBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this)
	{
		ServerEndOverlap(OtherActor);
	}
}

void ABossAttackZoneBase::ServerBeginOverlap_Implementation(AActor* OtherActor)
{
	// 보스는 Character 형이기 때문에 겹칠 수 있으니 수정해야함.
	ACharacter* OverlappingPlayer = Cast<ACharacter>(OtherActor);
	if ( !EnemyBossBase->GetIsBossWakeupTriggered() && OverlappingPlayer && OverlappingPlayer->ActorHasTag("Player"))
	{
		if (IsValid(EnemyBossBase))	// 값이 유효하면
		{
			EnemyBossBase->PlayRoarMontage();
			EnemyBossBase->SetIsBossWakeupTriggered(true);
		}
	}
}

void ABossAttackZoneBase::ServerEndOverlap_Implementation(AActor* OtherActor)
{
	ACharacter* OverlappingPlayer = Cast<ACharacter>(OtherActor);
	if (OverlappingPlayer && OverlappingPlayer->ActorHasTag("Player"))
	{

	}
}

// Called when the game starts or when spawned
void ABossAttackZoneBase::BeginPlay()
{
	Super::BeginPlay();
	
	AActor* EnemyBoss = UGameplayStatics::GetActorOfClass(this, AEnemyBossBase::StaticClass());
	EnemyBossBase = Cast<AEnemyBossBase>(EnemyBoss);
	
}

// Called every frame
void ABossAttackZoneBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

