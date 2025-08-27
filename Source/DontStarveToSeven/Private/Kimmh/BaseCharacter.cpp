// Fill out your copyright notice in the Description page of Project Settings.


#include "Kimmh/BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"


// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ĳ���� �����Ʈ ������Ʈ�� �����̼� ���� ��� 
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	// ĸ�� ������Ʈ
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));

	// MovementComponent �⺻ ����
	GetCharacterMovement()->bOrientRotationToMovement = true; // �̵� �������� ĳ���� ������ ȸ���ϵ��� ����
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.0f); // ĳ���� ȸ�� ���� ����
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;

	// SekeletalMesh ����
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.f, -92.48f), FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	// SekeletalMesh�� �⺻������ �� �޽� ����
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("SkeletalMesh'/Game/Assets/Characters/Mannequins/Meshes/SKM_Quinn'"));
	
	if (CharacterMeshRef.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
		UE_LOG(LogTemp, Log, TEXT("MeshCallSuccess"));
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("MeshCallFail"));
	}

}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


