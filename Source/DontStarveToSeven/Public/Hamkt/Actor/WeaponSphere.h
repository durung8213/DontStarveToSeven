// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hamkt/Actor/WeaponBase.h"
#include "WeaponSphere.generated.h"

class USphereComponent;
/**
 * 
 */
UCLASS()
class DONTSTARVETOSEVEN_API AWeaponSphere : public AWeaponBase
{
	GENERATED_BODY()
public:
	AWeaponSphere();
//
//    // 히트 이벤트 처리함수
//    UFUNCTION()
//    void OnSphereHit(
//        UPrimitiveComponent* HitComp,
//        AActor* OtherActor,
//        UPrimitiveComponent* OtherComp,
//        FVector NormalImpulse,
//        const FHitResult& Hit
//    );
//
//protected:
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
//	class UProjectileMovementComponent* ProjectileMovement;
//
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
//    USphereComponent* SphereCollision;
};
