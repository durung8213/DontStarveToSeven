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

	// Collision ������Ʈ ���� �� ����, RootComponent�� ����
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->InitSphereRadius(8.f);
	CollisionComp->SetCollisionProfileName(TEXT("Projectile"));
	RootComponent = CollisionComp;
	CollisionComp->OnComponentHit.AddDynamic(this, &ABullet::OnHit);

	// Mesh Component ���� �� ����
	MeshComp1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp1"));
	MeshComp1->SetupAttachment(RootComponent);
	MeshComp1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp1->SetSimulatePhysics(false);	// �Ѿ��� ���� �ùķ��̼��� ����

	// MeshComp2 ���� �� ����
	MeshComp2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp2"));
	MeshComp2->SetupAttachment(RootComponent);
	MeshComp2->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp1->SetSimulatePhysics(false);

	// Projectile Movement Component ���� �� �⺻ �ӵ� ����
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("BulletMovement"));
	ProjectileMovement->InitialSpeed = 2000.f;
	ProjectileMovement->MaxSpeed = 2000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;	
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 1.0f;

	// �⺻ ���ط��� �ִ� �̵� �Ÿ� ����
	Damage = 5.5f;
	MaxDistance = 5000.0f;	// 5000���� �̻� �̵��ϸ� �ı�

	//�ð� ��� �ı��� ���� 
	//InitialLifeSpan = 10.f;	//10�� �� �ı�
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

	//��ź ���� ��ġ ���
	SpawnLocation = GetActorLocation();

	if (GetOwner())
	{
		// ���ʰ� ���� ��, ��ź�� ������ �� ���ʸ� �����ϵ��� ����.
		CollisionComp->IgnoreActorWhenMoving(GetOwner(), true);
	}

	// �Ѿ��� �ʱ� �ӵ��� �����մϴ�.
	// ���⼭�� ������ ���� ������ ����Ͽ� �ӵ��� �ο��մϴ�.
	/*if (ProjectileMovement)
	{
		ProjectileMovement->Velocity = GetActorForwardVector() * ProjectileMovement->InitialSpeed;
	}*/


}


// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ��ź�� �̵��� �Ÿ��� üũ�Ͽ�, MaxDistance�� �ʰ��ϸ� �ı�
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
		FVector SpawnScale = FVector(0.01f);	// �ʿ信 ���� ������ ����

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),			//���� ���ؽ�Ʈ
			ExplosionEffect,	// ������ ���̾ư��� �ý���
			SLocaiton,		    // ���� ��ġ
			SpawnRotation,		// ���� ȸ����
			SpawnScale,			// ���� ������
			true,				// �ڵ����� �ı� ����
			true,				//����� Ȱ��ȭ ����
			ENCPoolMethod::None,//Ǯ�� ���
			true				// �ùķ��̼� Ȱ��ȭ ����
		);
	}
}


void ABullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	if (!HasAuthority() || !OtherActor || OtherActor == this)
	{
		return;
	}

	// �÷��̾��� ��� ����
	if (OtherActor->IsA(ABasePlayerCharacter::StaticClass()))
	{
		// ��ƼŬ ������
		Multicast_SpawnExplosionEffect();

		//�浹 �� ��ź �ı�
		Destroy();
		return;
	}


	// ���������� ó�� : �浹�� ����� ��ȿ�ϰ� �ڽ��� �ƴ� ��,
	// ApplyDamage�� ���� ���ظ� ����
	if (OtherActor->ActorHasTag(TEXT("Enemy")))
	{
		UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, UDamageType::StaticClass());
		IBPI_EnemyAI* Interface = Cast<IBPI_EnemyAI>(OtherActor);
		if (Interface) {
			// �������̽��� ������ ������ ��
			Interface->Execute_TakeDamage(OtherActor, Damage, GetParentActor());
		}
	}

	// ��ƼŬ ������
	Multicast_SpawnExplosionEffect();

	//�浹 �� ��ź �ı�
	Destroy();
	UE_LOG(LogTemp, Warning, TEXT("Destroy Bullet"));
}