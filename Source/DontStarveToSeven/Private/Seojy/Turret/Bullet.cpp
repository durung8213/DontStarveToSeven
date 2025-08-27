// Fill out your copyright notice in the Description page of Project Settings.
#include "Seojy/Turret/Bullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "NiagaraFunctionLibrary.h"
#include "Hamkt/Enemy/AI/BPI_EnemyAI.h"
#include "Kimmh/BasePlayerCharacter.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicatingMovement(true);

	// Collision 컴포넌트 생성 및 설정, RootComponent로 지정
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->InitSphereRadius(8.f);
	CollisionComp->SetCollisionProfileName(TEXT("Projectile"));
	RootComponent = CollisionComp;
	CollisionComp->OnComponentHit.AddDynamic(this, &ABullet::OnHit);

	// Mesh Component 생성 및 설정
	MeshComp1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp1"));
	MeshComp1->SetupAttachment(RootComponent);
	MeshComp1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp1->SetSimulatePhysics(false);	// 총알의 물리 시뮬레이션은 해제

	// MeshComp2 생성 및 설정
	MeshComp2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp2"));
	MeshComp2->SetupAttachment(RootComponent);
	MeshComp2->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp1->SetSimulatePhysics(false);

	// Projectile Movement Component 생성 및 기본 속도 설정
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("BulletMovement"));
	ProjectileMovement->InitialSpeed = 2000.f;
	ProjectileMovement->MaxSpeed = 2000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;	
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 1.0f;

	// 기본 피해량과 최대 이동 거리 설정
	Damage = 5.5f;
	MaxDistance = 5000.0f;	// 5000유닛 이상 이동하면 파괴

	//시간 기반 파괴도 가능 
	//InitialLifeSpan = 10.f;	//10초 뒤 파괴
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

	//포탄 생성 위치 기록
	SpawnLocation = GetActorLocation();

	if (GetOwner())
	{
		// 오너가 있을 때, 포탄이 움직일 때 오너를 무시하도록 설정.
		CollisionComp->IgnoreActorWhenMoving(GetOwner(), true);
	}

	// 총알의 초기 속도를 설정합니다.
	// 여기서는 액터의 전방 방향을 사용하여 속도를 부여합니다.
	/*if (ProjectileMovement)
	{
		ProjectileMovement->Velocity = GetActorForwardVector() * ProjectileMovement->InitialSpeed;
	}*/


}


// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 포탄이 이동한 거리를 체크하여, MaxDistance를 초과하면 파괴
	if ((GetActorLocation() - SpawnLocation).SizeSquared() > FMath::Square(MaxDistance))
	{
		Destroy();
		UE_LOG(LogTemp, Warning, TEXT("Destory Bullet"));
	}
}

void ABullet::Multicast_SpawnExplosionEffect_Implementation()
{
	if (ExplosionEffect)
	{
		FVector SLocaiton = GetActorLocation();
		FRotator SpawnRotation = GetActorRotation();
		FVector SpawnScale = FVector(0.01f);	// 필요에 따라 스케일 조정

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),			//월드 컨텍스트
			ExplosionEffect,	// 실행할 나이아가라 시스템
			SLocaiton,		    // 스폰 위치
			SpawnRotation,		// 스폰 회전값
			SpawnScale,			// 스폰 스케일
			true,				// 자동으로 파괴 여부
			true,				//오디오 활성화 여부
			ENCPoolMethod::None,//풀링 방식
			true				// 시뮬레이션 활성화 여부
		);
	}
}


void ABullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	if (!HasAuthority() || !OtherActor || OtherActor == this)
	{
		return;
	}

	// 플레이어일 경우 무시
	if (OtherActor->IsA(ABasePlayerCharacter::StaticClass()))
	{
		// 파티클 터지기
		Multicast_SpawnExplosionEffect();

		//충돌 후 포탄 파괴
		Destroy();
		return;
	}


	// 서버에서만 처리 : 충돌한 대상이 유효하고 자신이 아닐 때,
	// ApplyDamage를 통해 피해를 적용
	if (OtherActor->ActorHasTag(TEXT("Enemy")))
	{
		UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, UDamageType::StaticClass());
		IBPI_EnemyAI* Interface = Cast<IBPI_EnemyAI>(OtherActor);
		if (Interface) {
			// 인터페이스가 있으면 데미지 줌
			Interface->Execute_TakeDamage(OtherActor, Damage, GetParentActor());
		}
	}

	// 파티클 터지기
	Multicast_SpawnExplosionEffect();

	//충돌 후 포탄 파괴
	Destroy();
	UE_LOG(LogTemp, Warning, TEXT("Destroy Bullet"));
}