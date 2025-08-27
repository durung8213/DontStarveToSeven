// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "BossArmCapsuleSweepNotifyState.generated.h"

UCLASS()
class DONTSTARVETOSEVEN_API UBossArmCapsuleSweepNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	// AnimNotifyState�� �������̵� �Լ�
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	// ���� ĸ�� ������ �����ϴ� �Լ�
	UFUNCTION()
	void PerformCapsuleSweep(UWorld* World, AActor* OwnerActor, const FVector& StartPos, const FVector& EndPos);

protected:	
	// ������ ���� ������ - ���� �β�. �����ϸ��� * ������ �ؾ���.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CapsuleSweep")
	float CapsuleRadius = 15.f;

	// ���� ������
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CapsuleSweep")
	float AttackDamage = 25.f;

	// ������ ����� Ʈ���̽� ä��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CapsuleSweep")
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Visibility;

	// ���� ���� �̹� �������� �� ���� ��� ( �ߺ� �ǰ� ����! )
	TSet<AActor*> AlreadyHitActors;

	// �� ���� ����� ���� ������ �����ϱ� ���� ���
	TArray<TPair<FName, FName>> ArmSegments;

	

private:
	
	float AccumulatedTime = 0.0f;
};
