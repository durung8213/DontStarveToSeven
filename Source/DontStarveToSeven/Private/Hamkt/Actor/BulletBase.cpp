// Fill out your copyright notice in the Description page of Project Settings.


#include "Hamkt/Actor/BulletBase.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Hamkt/Enemy/AI/BPI_EnemyAI.h"


// Sets default values
ABulletBase::ABulletBase()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicatingMovement(true);

	// Collision 컴포넌트 생성 및 설정, RootComponent로 지정
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->InitSphereRadius(8.f);
	CollisionComp->SetCollisionProfileName(TEXT("Projectile"));
	RootComponent = CollisionComp;

	CollisionComp->OnComponentHit.AddDynamic(this, &ABulletBase::OnHit);

	// Mesh Component 생성 및 설정
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetSimulatePhysics(false);	// 발사체의 물리 시뮬레이션은 해제

	// ParticleSystemComp 생성 및 설정
	ParticleSystemComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComp"));
	ParticleSystemComp->SetupAttachment(RootComponent);


	// Projectile Movement Component 생성 및 기본 속도 설정
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("BulletMovement"));

	// 기본 피해량과 최대 이동 거리 설정
	Damage = 7.0f;

}

void ABulletBase::Multicast_SpawnExplosionEffect_Implementation()
{
	if (ExplosionParticle)
	{
		FVector SpawnLocation = GetActorLocation();
		FRotator SpawnRotation = GetActorRotation();
		FVector SpawnScale = FVector(1.5f); // 필요에 따라 스케일 조정

		// UGameplayStatics::SpawnEmitterAtLocation을 이용해 파티클 이펙트를 스폰합니다.
		// 마지막 인자인 bAutoDestroy가 true이면, 파티클이 완료되면 자동으로 파괴됩니다.
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),          // 월드 컨텍스트
			ExplosionParticle,   // 실행할 파티클 시스템
			SpawnLocation,       // 스폰 위치
			SpawnRotation,       // 스폰 회전
			SpawnScale,          // 스폰 스케일
			true                 // bAutoDestroy: true면 파티클이 끝난 후 자동 파괴
		);
	}
}

// Called when the game starts or when spawned
void ABulletBase::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner())
	{
		// 오너가 있을 때, 포탄이 움직일 때 오너를 무시하도록 설정.
		CollisionComp->IgnoreActorWhenMoving(GetOwner(), true);
	}
}

void ABulletBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// 서버에서만 처리 : 충돌한 대상이 유효하고 자신이 아닐 때, 영역이 아닐 때.
	if (HasAuthority() && OtherActor && OtherActor != this && !OtherActor->ActorHasTag(TEXT("Zone")))
	{
		// 액터가 해당 인터페이스를 구현하는가?
		if (   (OtherActor->ActorHasTag(TEXT("Player")) || OtherActor->ActorHasTag(TEXT("Craft")))
			&& OtherActor->GetClass()->ImplementsInterface(UBPI_EnemyAI::StaticClass()) )
		{
			FString Message = FString::Printf(TEXT("Boss 7F ATTACK !!!"));
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, Message);
			IBPI_EnemyAI::Execute_TakeDamage(OtherActor, 7.f, this);	// HitActor 된 캐릭의 TakeDamage 함수를 가져와서 7 데미지를 입힌다.
		}
		// 파티클 터지기
		Multicast_SpawnExplosionEffect();

		//충돌 후 포탄 파괴
		Destroy();
		UE_LOG(LogTemp, Warning, TEXT("Destroy Bullet"));
	}
}


