#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Hamkt/Enemy/AI/Enums/E_MovementSpeed.h"
#include "BPI_EnemyAI.generated.h"


/**
 * UBPI_EnemyAI
 * - 이 클래스는 리플렉션 시스템에 노출되는 UObject 기반 인터페이스입니다.
 */
UINTERFACE(Blueprintable)
class DONTSTARVETOSEVEN_API UBPI_EnemyAI : public UInterface
{
	GENERATED_BODY()
};

/**
 * IBPI_EnemyAI
 * - 실제 인터페이스 함수들을 선언하는 순수 C++ 인터페이스 클래스입니다.
 *   이 인터페이스는 Blueprint와 C++ 모두에서 구현할 수 있습니다.
 */
class DONTSTARVETOSEVEN_API IBPI_EnemyAI
{
	GENERATED_BODY()

public:
	/**
	 * Blueprint에서 구현 가능한 함수 예시.
	 * BlueprintNativeEvent를 사용하면 기본 C++ 구현을 제공할 수 있으며, Blueprint에서 오버라이드할 수도 있습니다.
	 */

	// 상황별 스피드
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "EnemyAI")
	void SetMovementSpeed(E_MovementSpeed NewSpeed);

	// 사거리
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "EnemyAI")
	void GetIdealRange(float& AttackRadius, float& DefendRadius) const;

	// 장비 장착
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Common")
	void Equip(FName NotifyName);

	// 장비 탈착
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Common")
	void UnEquip();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Common")
	void Attack(FName NotifyName);

	// 투사체 로직 함수
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Common")
	void ThrowWeaponAtTarget(AActor* InWeapon, AActor* InTarget);

	// 현재 체력을 가져오는 함수
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stat")
	float GetCurrentHealth();

	// 최대 체력을 가져오는 함수
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stat")
	float GetMaxHealth();

	// 데미지를 입는다면
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stat")
	void TakeDamage(float Amount, AActor* DamageActor);

	// 죽었는지?
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stat")
	bool IsDead();


	// 추후 예정 Drop , Heal, ...
};