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
	// AnimNotifyState의 오버라이드 함수
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	// 실제 캡슐 스윕을 수행하는 함수
	UFUNCTION()
	void PerformCapsuleSweep(UWorld* World, AActor* OwnerActor, const FVector& StartPos, const FVector& EndPos);

protected:	
	// 스윕에 사용될 반지름 - 팔의 두께. 스케일마다 * 적용을 해야함.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CapsuleSweep")
	float CapsuleRadius = 15.f;

	// 공격 데미지
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CapsuleSweep")
	float AttackDamage = 25.f;

	// 스윕에 사용할 트레이스 채널
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CapsuleSweep")
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Visibility;

	// 공격 도중 이미 데미지를 준 액터 목록 ( 중복 피격 방지! )
	TSet<AActor*> AlreadyHitActors;

	// 각 팔의 연결된 소켓 구간을 스윕하기 위한 목록
	TArray<TPair<FName, FName>> ArmSegments;

	

private:
	
	float AccumulatedTime = 0.0f;
};
