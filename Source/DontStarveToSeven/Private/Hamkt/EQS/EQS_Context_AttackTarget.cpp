// Fill out your copyright notice in the Description page of Project Settings.


#include "Hamkt/EQS/EQS_Context_AttackTarget.h"
#include "AIController.h"
#include "Hamkt/Enemy/AI/AIC_EnemyBase.h"
#include "GameFramework/Actor.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
// 테스트용
#include "GameFramework/PlayerStart.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/World.h"
#include "EngineUtils.h" // TActorIterator 사용을 위한 헤더


UEQS_Context_AttackTarget::UEQS_Context_AttackTarget()
{
}

// AI가 타겟으로 지정한 액터를 기반으로 EQS 실행
void UEQS_Context_AttackTarget::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	// 쿼리를 실행할 액터를 가져옴
	AActor* QuerierActor = Cast<AActor>(QueryInstance.Owner.Get());

	
	/**   For Testing Only!!!!  실행 할 땐 주석처리 **/
	/**   For Testing Only!!!!  실행 할 땐 주석처리 **/

	//UWorld* World = QuerierActor->GetWorld();
	//if (!World)
	//{
	//	return;
	//}

	//// 디버깅 메시지 출력 (블루프린트의 Print String 대체)
	//UKismetSystemLibrary::PrintString(World, TEXT("TESTING PURPOSES ONLY!!!"), true, true, FColor::Yellow, 2.0f);

	//// 1. APlayerStart 가져오기 (블루프린트의 Get Actor Of Class)
	//for (TActorIterator<APlayerStart> It(World); It; ++It)
	//{
	//	UEnvQueryItemType_Actor::SetContextHelper(ContextData, *It);
	//	return;  // 첫 번째 찾은 PlayerStart 반환
	//}

	/**   For Testing Only!!!!  END LINE **/
	/**   For Testing Only!!!!  END LINE **/



	if (QuerierActor)
	{
		// AI 컨트롤러를 가져옴
		AAIController* AIController = Cast<AAIController>(QuerierActor->GetInstigatorController());
		if (AIController)
		{
			// AIC_EnemyBase로 캐스팅
			AAIC_EnemyBase* EnemyAIController = Cast<AAIC_EnemyBase>(AIController);
			if (EnemyAIController)
			{
				AActor* AttackTarget = EnemyAIController->GetAttackTargetActor();	// AIController에서 공격대상 정보를 가져옴
				if (AttackTarget)
				{
					// EQS에서 Context로 사용할 액터 등록
					UEnvQueryItemType_Actor::SetContextHelper(ContextData, AttackTarget);
				}
			}
		}
	}
}
