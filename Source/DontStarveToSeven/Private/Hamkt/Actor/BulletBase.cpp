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

	// Collision ������Ʈ ���� �� ����, RootComponent�� ����
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->InitSphereRadius(8.f);
	CollisionComp->SetCollisionProfileName(TEXT("Projectile"));
	RootComponent = CollisionComp;

	CollisionComp->OnComponentHit.AddDynamic(this, &ABulletBase::OnHit);

	// Mesh Component ���� �� ����
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetSimulatePhysics(false);	// �߻�ü�� ���� �ùķ��̼��� ����

	// ParticleSystemComp ���� �� ����
	ParticleSystemComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComp"));
	ParticleSystemComp->SetupAttachment(RootComponent);


	// Projectile Movement Component ���� �� �⺻ �ӵ� ����
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("BulletMovement"));

	// �⺻ ���ط��� �ִ� �̵� �Ÿ� ����
	Damage = 7.0f;

}

void ABulletBase::Multicast_SpawnExplosionEffect_Implementation()
{
	if (ExplosionParticle)
	{
		FVector SpawnLocation = GetActorLocation();
		FRotator SpawnRotation = GetActorRotation();
		FVector SpawnScale = FVector(1.5f); // �ʿ信 ���� ������ ����

		// UGameplayStatics::SpawnEmitterAtLocation�� �̿��� ��ƼŬ ����Ʈ�� �����մϴ�.
		// ������ ������ bAutoDestroy�� true�̸�, ��ƼŬ�� �Ϸ�Ǹ� �ڵ����� �ı��˴ϴ�.
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),          // ���� ���ؽ�Ʈ
			ExplosionParticle,   // ������ ��ƼŬ �ý���
			SpawnLocation,       // ���� ��ġ
			SpawnRotation,       // ���� ȸ��
			SpawnScale,          // ���� ������
			true                 // bAutoDestroy: true�� ��ƼŬ�� ���� �� �ڵ� �ı�
		);
	}
}

// Called when the game starts or when spawned
void ABulletBase::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner())
	{
		// ���ʰ� ���� ��, ��ź�� ������ �� ���ʸ� �����ϵ��� ����.
		CollisionComp->IgnoreActorWhenMoving(GetOwner(), true);
	}
}

void ABulletBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// ���������� ó�� : �浹�� ����� ��ȿ�ϰ� �ڽ��� �ƴ� ��, ������ �ƴ� ��.
	if (HasAuthority() && OtherActor && OtherActor != this && !OtherActor->ActorHasTag(TEXT("Zone")))
	{
		// ���Ͱ� �ش� �������̽��� �����ϴ°�?
		if (   (OtherActor->ActorHasTag(TEXT("Player")) || OtherActor->ActorHasTag(TEXT("Craft")))
			&& OtherActor->GetClass()->ImplementsInterface(UBPI_EnemyAI::StaticClass()) )
		{
			FString Message = FString::Printf(TEXT("Boss 7F ATTACK !!!"));
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, Message);
			IBPI_EnemyAI::Execute_TakeDamage(OtherActor, 7.f, this);	// HitActor �� ĳ���� TakeDamage �Լ��� �����ͼ� 7 �������� ������.
		}
		// ��ƼŬ ������
		Multicast_SpawnExplosionEffect();

		//�浹 �� ��ź �ı�
		Destroy();
		UE_LOG(LogTemp, Warning, TEXT("Destroy Bullet"));
	}
}


