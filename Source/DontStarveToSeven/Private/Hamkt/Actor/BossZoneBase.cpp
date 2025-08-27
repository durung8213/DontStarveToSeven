// Fill out your copyright notice in the Description page of Project Settings.


#include "Hamkt/Actor/BossZoneBase.h"
#include "Components/SphereComponent.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "Hamkt/Enemy/EnemyBoss/EnemyBossBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kimmh/Character/Controller/DTS_GamePlayPlayerController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

// Sets default values
ABossZoneBase::ABossZoneBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	Tags.Add(TEXT("Zone"));

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	RootComponent = CollisionSphere;
	CollisionSphere->SetGenerateOverlapEvents(true);

	// Overlap 이벤트 바인딩
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ABossZoneBase::OnOverlapBegin);
	CollisionSphere->OnComponentEndOverlap.AddDynamic(this, &ABossZoneBase::OnOverlapEnd);
}

// 서버 멀티 작업 안되어있음

TSet<ACharacter*> ABossZoneBase::GetOverlappingPlayers()
{
	return OverlappingPlayers;
}

// Called when the game starts or when spawned
void ABossZoneBase::BeginPlay()
{
	Super::BeginPlay();

	AActor* EnemyBossBaseIn = UGameplayStatics::GetActorOfClass(this, AEnemyBossBase::StaticClass());
	EnemyBossBase = Cast<AEnemyBossBase>(EnemyBossBaseIn);
	if (EnemyBossBase)
	{
		EnemyBossBase->ServerSetBlackboardKey();
	}
	
}

// 서버랑 멀티 제작해야함
void ABossZoneBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		ServerBeginOverlap(OtherActor);
	}
}

void ABossZoneBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this)
	{
		ServerEndOverlap(OtherActor);
	}
}

void ABossZoneBase::ServerBeginOverlap_Implementation(AActor* OtherActor)
{
	ACharacter* OverlappingPlayer = Cast<ACharacter>(OtherActor);
	if (OverlappingPlayer && OverlappingPlayer->ActorHasTag("Player"))
	{
		OverlappingPlayers.Add(OverlappingPlayer);	// TSet 자료구조에 값을 추가
		UE_LOG(LogTemp, Log, TEXT("Player entered area: %s"), *OverlappingPlayer->GetName());

		if (EnemyBossBase)
		{
			ADTS_GamePlayPlayerController* CurrentPlayerController = Cast<ADTS_GamePlayPlayerController>(OverlappingPlayer->GetController());
			if (CurrentPlayerController) {
				CurrentPlayerController->ClientSetBoss(EnemyBossBase);
			}
		}
		
	}
}

void ABossZoneBase::ServerEndOverlap_Implementation(AActor* OtherActor)
{
	ACharacter* OverlappingPlayer = Cast<ACharacter>(OtherActor);
	if (OverlappingPlayer && OverlappingPlayer->ActorHasTag("Player"))
	{
		OverlappingPlayers.Remove(OverlappingPlayer);
		UE_LOG(LogTemp, Log, TEXT("Player left area: %s"), *OverlappingPlayer->GetName());

		if (EnemyBossBase)
		{
			ADTS_GamePlayPlayerController* CurrentPlayerController = Cast<ADTS_GamePlayPlayerController>(OverlappingPlayer->GetController());
			if (CurrentPlayerController) {
				CurrentPlayerController->ClientUnSetBoss(EnemyBossBase);
			}
		}

		if (OverlappingPlayers.Num() <= 0)
		{
			EnemyBossBase->Server_ResetBehavior_Implementation();
			EnemyBossBase->SetIsBossWakeupTriggered(false);			// 보스는 다시 잠든 상태로 변환
		}
	}
}

// Called every frame
void ABossZoneBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

