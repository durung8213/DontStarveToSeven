// Fill out your copyright notice in the Description page of Project Settings.


#include "Hamkt/EQS/EQS_Context_AttackTarget.h"
#include "AIController.h"
#include "Hamkt/Enemy/AI/AIC_EnemyBase.h"
#include "GameFramework/Actor.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
// �׽�Ʈ��
#include "GameFramework/PlayerStart.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/World.h"
#include "EngineUtils.h" // TActorIterator ����� ���� ���


UEQS_Context_AttackTarget::UEQS_Context_AttackTarget()
{
}

// AI�� Ÿ������ ������ ���͸� ������� EQS ����
void UEQS_Context_AttackTarget::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	// ������ ������ ���͸� ������
	AActor* QuerierActor = Cast<AActor>(QueryInstance.Owner.Get());

	
	/**   For Testing Only!!!!  ���� �� �� �ּ�ó�� **/
	/**   For Testing Only!!!!  ���� �� �� �ּ�ó�� **/

	//UWorld* World = QuerierActor->GetWorld();
	//if (!World)
	//{
	//	return;
	//}

	//// ����� �޽��� ��� (�������Ʈ�� Print String ��ü)
	//UKismetSystemLibrary::PrintString(World, TEXT("TESTING PURPOSES ONLY!!!"), true, true, FColor::Yellow, 2.0f);

	//// 1. APlayerStart �������� (�������Ʈ�� Get Actor Of Class)
	//for (TActorIterator<APlayerStart> It(World); It; ++It)
	//{
	//	UEnvQueryItemType_Actor::SetContextHelper(ContextData, *It);
	//	return;  // ù ��° ã�� PlayerStart ��ȯ
	//}

	/**   For Testing Only!!!!  END LINE **/
	/**   For Testing Only!!!!  END LINE **/



	if (QuerierActor)
	{
		// AI ��Ʈ�ѷ��� ������
		AAIController* AIController = Cast<AAIController>(QuerierActor->GetInstigatorController());
		if (AIController)
		{
			// AIC_EnemyBase�� ĳ����
			AAIC_EnemyBase* EnemyAIController = Cast<AAIC_EnemyBase>(AIController);
			if (EnemyAIController)
			{
				AActor* AttackTarget = EnemyAIController->GetAttackTargetActor();	// AIController���� ���ݴ�� ������ ������
				if (AttackTarget)
				{
					// EQS���� Context�� ����� ���� ���
					UEnvQueryItemType_Actor::SetContextHelper(ContextData, AttackTarget);
				}
			}
		}
	}
}
