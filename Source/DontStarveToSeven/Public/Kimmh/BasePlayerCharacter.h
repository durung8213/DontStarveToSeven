// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kimmh/BaseCharacter.h"
#include "Hamkt/Enemy/AI/BPI_EnemyAI.h"
#include "Kimmh/Enum/IkUseEnum.h"
#include "Kimmh/Enum/UpperBodyOverlayEnum.h"
#include "BasePlayerCharacter.generated.h"

/**
 *
 */

UENUM(BlueprintType)
enum class EDirection : uint8 {
	Forward UMETA(DisplayName = "Forward"),
	Back UMETA(DisplayName = "Back"),
	Left UMETA(DisplayName = "Left"),
	Right UMETA(DisplayName = "Right")
};

UENUM(BlueprintType)
enum class ETool : uint8 {
	WoodAxe UMETA(DisplayName = "WoodAxe"),
	PickAxe UMETA(DisplayName = "PickAxe"),
	Sickle UMETA(DisplayName = "Sickle"),
};

UCLASS()
class DONTSTARVETOSEVEN_API ABasePlayerCharacter : public ABaseCharacter, public IBPI_EnemyAI
{
	GENERATED_BODY()

public:
	ABasePlayerCharacter();

	virtual void BeginPlay() override;

	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/////////////////////////////////////
	// ������ ���� �Լ�
	// ���� ü�� ����
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Stat")
	float GetCurrentHealth();
	virtual float GetCurrentHealth_Implementation() override;

	// �ִ�ü�� ����
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Stat")
	float GetMaxHealth();
	virtual float GetMaxHealth_Implementation() override;

	// �������� ������ ȣ��
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Stat")
	void TakeDamage(const float Amount, AActor* DamageActor);
	virtual void TakeDamage_Implementation(float Amount, AActor* DamageActor) override;

	UFUNCTION(Server, Reliable)
	void SeverTakeBossDamage(float Amount, AActor* DamageActor);
	void SeverTakeBossDamage_Implementation(float Amount, AActor* DamageActor);

	

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Stat")
	bool IsDead();
	virtual bool IsDead_Implementation() override;


	// �޺��� ���� �ϴ� �Լ�
	UFUNCTION(BlueprintCallable)
	void ResetComboCount();

	// ���ݺҰ����¿��� ���� ���� ���·� �����ϴ� �Լ�
	UFUNCTION(BlueprintCallable)
	void ResetbCanAttack();

	// Ȱ ����� �ʱ�ȭ ��
	void ResetbIsAiming();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerMontagePlayWithSession(UAnimMontage* Montage, FName SectionName, float PlayRate = 1.3f);

	UFUNCTION(NetMulticast, Reliable)
	void MultiMontagePlayWithSession(UAnimMontage* Montage, FName SectionName, float PlayRate = 1.3f);

	/// <summary>
	/// ��Ÿ�ָ� ����ϴ� �Լ�
	/// </summary>
	/// <param name="Montage">����� ��Ÿ��</param>
	/// <param name="SectionName">���� �̸�</param>
	UFUNCTION(BlueprintCallable)
	void PlayMontageWithSession(UAnimMontage* Montage, FName SectionName, float PlayRate = 1.3f);

	///----------------- ���� ���� ---------------------

	// �������� Attack�� �ϴ� �Լ�
	UFUNCTION(Server, Reliable)
	void ServerAttack();

	// ��Ƽ���� Attack�� �ϴ� �Լ�
	UFUNCTION(NetMulticast, Reliable)
	void MultiAttack();

	void ResetAttackRequest();

	// �������� �������� �϶�� �����ϴ� �Լ�
	UFUNCTION(Server, Reliable)
	void ServerStrongAttack();

	// ��Ƽ���� �������� �϶�� �����ϴ� �Լ�
	UFUNCTION(NetMulticast, Reliable)
	void MultiStrongAttack();

	// �������� Ȱ ������ �ϴ� �Լ�
	UFUNCTION(Server, Reliable)
	void ServerBowAiming();

	// �������� Ȱ�� ��� �Լ�
	UFUNCTION(Server, Reliable)
	void ServerBowShot();

	// ��Ƽ���� �������� �϶�� �����ϴ� �Լ�
	UFUNCTION(NetMulticast, Reliable)
	void MultiBowShotMontagePlay();

	// �������� Ȱ ������ ����ϴ� �Լ�
	UFUNCTION(Server, Reliable)
	void ServerBowAimReleased();

	///----------------- ���� ���� �� ---------------------

	///----------------- ��Ʈ ���� ---------------------

	UFUNCTION(Server, Reliable)
	void ServerHit(EDirection Direction = EDirection::Forward);

	UFUNCTION(NetMulticast, Reliable)
	void MultiHit(EDirection Direction = EDirection::Forward);

	///----------------- ��Ʈ ���� �� ---------------------

	///----------------- ������ ���� -------------------
	/// <summary>
	/// �÷��̾� �¿� �������� ��Ÿ���� �Լ�
	/// </summary>
	/// <param name="NewMoveX">-1(��), 0(����) 1(��)</param>
	UFUNCTION(Server, Reliable)
	void ServerSetMoveX(float NewMoveX);

	/// <summary>
	/// �÷��̾� �յ� ������
	/// </summary>
	/// <param name="NewMoveY">-1(��), 0(����) 1(��)</param>
	UFUNCTION(Server, Reliable)
	void ServerSetMoveY(float NewMoveY);

	///----------------- ������ ���� �� -------------------

	void SetbOrientRotationToMovement(bool bOrientRotationToMovementValue);

	UFUNCTION(Server, Reliable)
	void ServerSetYaw(float NewYaw);

	UFUNCTION(Server, Reliable)
	void ServerSetPitch(float NewPitch);

	/// <summary>
	/// UpperOverlay�� �ٲٴ� �Լ�
	/// </summary>
	/// <param name="UpperOverlay">�ٲ� UpperOverlay</param>
	UFUNCTION(Server, Reliable)
	void ServerChangeUpperOverlay(EUpperOverlay UpperOverlay);


	/// <summary>
	/// ĸ�� ������
	/// </summary>
	/// <param name="OverlappedComp"></param>
	/// <param name="OtherActor"></param>
	/// <param name="OtherComp"></param>
	/// <param name="OtherBodyIndex"></param>
	/// <param name="bFromSweep"></param>
	/// <param name="SweepResult"></param>
	UFUNCTION()
	void OnCapsuleBeginOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	/// <summary>
	/// ĸ�� ��Ʈ
	/// </summary>
	/// <param name="HitComp"></param>
	/// <param name="OtherActor"></param>
	/// <param name="OtherComp"></param>
	/// <param name="NormalImpulse"></param>
	/// <param name="Hit"></param>
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/// <summary>
	/// �տ� ������ ����
	/// </summary>
	/// <param name="EquipItem">������ ������</param>
	void AttachItem(class ASHBaseItem* NewEquipItem);

	/// <summary>
	/// ������ Ż��
	/// </summary>
	void DetachItem();

	/// <summary>
	/// �ٸ� �� ��ġ�� ��ȯ�ϴ� �Լ�
	/// </summary>
	/// <returns>�ٸ��� Transform</returns>
	UFUNCTION(BlueprintCallable)
	FTransform GetAnotherHandTransform();

	UFUNCTION(BlueprintCallable) 
	void SetToolVisibility(ETool Tool, bool bIsVisibility);

	UFUNCTION(NetMulticast, Reliable)
	void MultiSetToolVisibility(ETool Tool, bool bIsVisibility);

	UFUNCTION(Server, Reliable)
	void ServerSetToolCollisionEnable(ETool Tool, bool bIsEnable);

	UFUNCTION(NetMulticast, Reliable)
	void MultiSetToolCollisionEnable(ETool Tool, bool bIsEnable);

	UFUNCTION(BlueprintCallable)
	void SetToolCollisionEnable(ETool Tool, bool bIsEnable);

	UFUNCTION(BlueprintCallable)
	bool GetIntractionActorCanNextActive();
	
	bool GetPickAxeHigh();

	bool GetPickAxeMiddle();

	UFUNCTION(Server, Reliable)
	void ServerDestory(AActor* Actor);

	/// <summary>
	///	������ ���ø�����Ʈ �ϱ� ���� �ʿ��� �Լ�(�����϶��)
	/// </summary>
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated, Category = "PlayerStance")
	float MoveX;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated, Category = "PlayerStance")
	float MoveY;

	/// <summary>
	/// ī�޶� ��
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	class USpringArmComponent* CameraBoom;

	/// <summary>
	/// ���� ������ ī�޶� ������Ʈ
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	class UCameraComponent* ExtraCamera;

	/// <summary>
	/// ���� �÷��̾��� ī�޶� �� ī�޶� ����
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	class ABaseCamera* ThirdPersonBaseCameraActor;

	/// <summary>
	/// ũ������ �޴��� ���� ������Ʈ
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UCraftManager* CraaftManager;

	/// <summary>
	/// ABaseCamera�� �����ڿ��� �ֱ� ���� ī�޶� ���� ������Ʈ
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UChildActorComponent* CameraComponent;

	/// ------------------- ���� ���� -----------------------------------------

	/// <summary>
	/// ī�޶� �� ���� // ���� ����
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	FVector CamreaForwardVector;

	/// <summary>
	/// �� ������ �����ϱ� ���� Enum
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "PlayerStance")
	EUpperOverlay UpperBodyOverlay;

	/// <summary>
	/// �޺� ī��Ʈ�� ���½����ִ� Ÿ�̸�
	/// </summary>
	FTimerHandle ComboCountResetTimer;

	/// <summary>
	/// 일반 공격 요청
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated, Category = "PlayerStance")
	bool bNormalAttackRequest = false;

	/// <summary>
	/// 강 공격 요청
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated, Category = "PlayerStance")
	bool bStrongAttackRequest = false;

	/// <summary>
	/// 일반 공격 요청
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated, Category = "PlayerStance")
	bool bCanAttack = true;

	bool bCanParrying = false;
	
	/// <summary>
	/// 콤보 카운트
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated, Category = "PlayerStance")
	int ComboCount;

	/// <summary>
	/// �������ΰ��� ��Ÿ���� ����
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated, Category = "PlayerStance")
	bool bIsAiming;

	/// <summary>
	/// ��� ��
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated, Category = "PlayerStance")
	float DrawAmount;

	/// ------------------- ���� ���� ��-----------------------------------------

	/// --------------------- ��Ÿ�� -------------------------------------

	// �� ���� ��Ÿ��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* UpperBodyMontage;

	// ��ü ���� ��Ÿ��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* FullBodyMontage;

	// 한손 공격 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* OneHandSword;

	// 양손 공격 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* TwoHandSword;

	// 창 공격 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* SpearMontage;

	// 활 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* BowMontage;

	// Ȱ �±� ���� ��Ÿ��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* HitMontage;

	// ��ȣ�ۿ� �뵵 ��Ÿ��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* InteractionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	TArray<UAnimMontage*> MontageArr;

	// FullBodySection�� �̸��� �� �ִ� �迭
	TArray<FName> FullBodySectionName;

	// UpperBodySection�� �̸��� �� �ִ� �迭
	TArray<FName> UpperBodySectionName;

	// UpperBodySection�� �̸��� �� �ִ� �迭
	TArray<FName> HitSectionName;

	// UpperBodySection�� �̸��� �� �ִ� �迭
	TArray<FName> InteractionSectionName;

	// FullBodySection�� �̸��� �� �ִ� �迭
	TArray<FName> OneHandSwordSectionName;

	// UpperBodySection�� �̸��� �� �ִ� �迭
	TArray<FName> TwoHandSectionName;

	// UpperBodySection�� �̸��� �� �ִ� �迭
	TArray<FName> SpearSectionName;

	/// --------------------- ��Ÿ�� ��-------------------------------------

	/// --------------------- �̵� ���� ���� -------------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated, Category = "PlayerStance")
	float Yaw;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated, Category = "PlayerStance")
	float Pitch;
	/// --------------------- �̵� ���� ���� �� -------------------------------

	/// --------------------- ��ȣ �ۿ� ���� ---------------------------------
	// ��ȣ�ۿ� �� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	AActor* CurrentInteractionActor;

	/// <summary>
	/// ���� ���� ������Ʈ
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tool")
	class UChildActorComponent* WoodAxeActorComponent;

	/// <summary>
	/// ��� ���� ������Ʈ
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tool")
	class UChildActorComponent* PickAxeActorComponent;

	/// <summary>
	/// �� ���� ������Ʈ
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tool")
	class UChildActorComponent* SickleActorComponent;

	class ADTS_BaseTool* WoodAxe;
	class ADTS_BaseTool* PickAxe;
	class ADTS_BaseTool* Sickle;

	bool bInteractionDoing = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	class USceneComponent* PickAxeMid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	class USceneComponent* PickAxeHigh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	class USceneComponent* WoodAxe1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	class USceneComponent* WoodAxe2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	class ASHBaseItem* CurrentItem;

	/// --------------------- ���� ���� ���� ---------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	class ASHBaseItem* CurrentEquipItem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	EIkUse IkUse = EIkUse::None;


};
