#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Hamkt/Enemy/AI/Enums/E_MovementSpeed.h"
#include "BPI_EnemyAI.generated.h"


/**
 * UBPI_EnemyAI
 * - �� Ŭ������ ���÷��� �ý��ۿ� ����Ǵ� UObject ��� �������̽��Դϴ�.
 */
UINTERFACE(Blueprintable)
class DONTSTARVETOSEVEN_API UBPI_EnemyAI : public UInterface
{
	GENERATED_BODY()
};

/**
 * IBPI_EnemyAI
 * - ���� �������̽� �Լ����� �����ϴ� ���� C++ �������̽� Ŭ�����Դϴ�.
 *   �� �������̽��� Blueprint�� C++ ��ο��� ������ �� �ֽ��ϴ�.
 */
class DONTSTARVETOSEVEN_API IBPI_EnemyAI
{
	GENERATED_BODY()

public:
	/**
	 * Blueprint���� ���� ������ �Լ� ����.
	 * BlueprintNativeEvent�� ����ϸ� �⺻ C++ ������ ������ �� ������, Blueprint���� �������̵��� ���� �ֽ��ϴ�.
	 */

	// ��Ȳ�� ���ǵ�
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "EnemyAI")
	void SetMovementSpeed(E_MovementSpeed NewSpeed);

	// ��Ÿ�
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "EnemyAI")
	void GetIdealRange(float& AttackRadius, float& DefendRadius) const;

	// ��� ����
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Common")
	void Equip(FName NotifyName);

	// ��� Ż��
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Common")
	void UnEquip();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Common")
	void Attack(FName NotifyName);

	// ����ü ���� �Լ�
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Common")
	void ThrowWeaponAtTarget(AActor* InWeapon, AActor* InTarget);

	// ���� ü���� �������� �Լ�
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stat")
	float GetCurrentHealth();

	// �ִ� ü���� �������� �Լ�
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stat")
	float GetMaxHealth();

	// �������� �Դ´ٸ�
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stat")
	void TakeDamage(float Amount, AActor* DamageActor);

	// �׾�����?
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stat")
	bool IsDead();


	// ���� ���� Drop , Heal, ...
};