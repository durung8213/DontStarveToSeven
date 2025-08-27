// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossZoneBase.generated.h"



UCLASS()
class DONTSTARVETOSEVEN_API ABossZoneBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABossZoneBase();

    UFUNCTION()
    TSet<ACharacter*> GetOverlappingPlayers();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

////////////////////////////////////////////////

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



 //////////////////////////////////
 // printReadOnly, Category = "Components")
    class USphereComponent* CollisionSphere;

    // [설명] 영역 내에 있는 플레이어들을 저장(중복 없이)하기 위해 TSet 사용.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay")
    TSet<ACharacter*> OverlappingPlayers;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
    class AEnemyBossBase* EnemyBossBase;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
