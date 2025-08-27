// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossAttackZoneBase.generated.h"

UCLASS()
class DONTSTARVETOSEVEN_API ABossAttackZoneBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABossAttackZoneBase();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class USphereComponent* CollisionSphere;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


    UFUNCTION(Server, Reliable)
    void ServerBeginOverlap(AActor* OtherActor);

    UFUNCTION(Server, Reliable)
    void ServerEndOverlap(AActor* OtherActor);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

   

    UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
    class AEnemyBossBase* EnemyBossBase;

};
