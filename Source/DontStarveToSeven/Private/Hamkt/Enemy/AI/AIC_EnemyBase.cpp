// Fill out your copyright notice in the Description page of Project Settings.


#include "Hamkt/Enemy/AI/AIC_EnemyBase.h"
#include "GameFramework/Pawn.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"              // AActor 선언
#include "TimerManager.h"					  // SetTimer 함수
#include "Engine/Engine.h"                    // UE_LOG 사용
#include "Hamkt/Enemy/AI/BPI_EnemyAI.h"
#include "Hamkt/Enemy/EnemyBase.h"

#include "Hamkt/Enemy/AI/Enums/E_AIState.h"
#include "Perception/AISense.h"                // FAIPerceptionInfo, FAIStimulus 선언 (필요에 따라)
#include "Perception/AIPerceptionTypes.h"		// FAIPerceptionInfo 선언
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"



AAIC_EnemyBase::AAIC_EnemyBase()
{
	//////////////////////////////////////////////////////////////////////////////
	// 
	// AI Perception Component 생성
	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));

	// 시야, 청각, 데미지 감지를 위한 SenseConfig 생성
	SightConfig	  =	CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	

	// SightConfig 속성 설정
	SightConfig->SightRadius = 800.f;								// AI가 시야를 통해 감지 할 수 있는 최대 거리
	SightConfig->LoseSightRadius = 1200.f;							// 감지된 객체가 해당 범위를 벗어나면 더 이상 시각적으로 인식하지 않음... 
																		// 그러므로 SightRadius 보다 큰 값이어야함.						
	SightConfig->PeripheralVisionAngleDegrees = 80.f;				// AI의 시야각 (한쪽씩)	
	SightConfig->SetMaxAge(5.f);									// 감지된 자극이 메모리에 남아있는 최대 시간(초)

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;		// 적군으로 분류된 객체를 감지할건지?
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;	// 아군으로 분류된 객체를 감지할건지?
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;		// 중립으로 분류된 객체를 감지할건지?

	// Perception Component에 시야, 청각, 데미지 감지 설정 추가
	AIPerceptionComp->ConfigureSense(*SightConfig);

	// Dominant Sense를 시각 감지로 설정 (AI가 우선적으로 시각 정보를 사용)
	AIPerceptionComp->SetDominantSense(UAISense_Sight::StaticClass());


	//////////////////////////////////////////////////////////////////////////////

	// OnPerceptionUpdate 이벤트에 함수 바인딩 (동적 멀티캐스트 델리게이트)
	if (AIPerceptionComp)
	{
		// 중복 등록되어 있지 않을 때만 추가
		AIPerceptionComp->OnPerceptionUpdated.RemoveDynamic(this, &AAIC_EnemyBase::OnPerceptionUpdate);
		AIPerceptionComp->OnPerceptionUpdated.AddDynamic(this, &AAIC_EnemyBase::OnPerceptionUpdate);
	}
}

void AAIC_EnemyBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	UE_LOG(LogTemp, Log, TEXT("AAIC_EnemyBase has possessed pawn : %s"), *InPawn->GetName());

	// 현재 비헤이비어트리 값이 유효한지?
	if (!IsValid(BehaviorTreeAsset)) return;

	if (!IsValid(InPawn)) return;

	AEnemyBase* EnemyBase = Cast<AEnemyBase>(InPawn);	// 현재 Pawn을 EnemyBase로 형변환
	if (!IsValid(EnemyBase)) return;

	if (!IsValid(EnemyBase->GetBehaviorTree())) return;	// 비헤이비어트리 값 있는지 확인
	
	
		// 런비헤이비어는 블루프린트에서 반환값이 리턴.. 그래서 if문으로 작업됨.
		if (RunBehaviorTree(EnemyBase->GetBehaviorTree()))
		{
			BlackboardComp = GetBlackboardComponent();		// 현재 블랙보드 컴포넌트 가져오기

			// 상태를 Passive로 설정
			SetStateAsPassive();

			if (InPawn->Implements<UBPI_EnemyAI>())
			{
				// 공격 사거리 / 방어 영역을 저장할 변수 선언
				float AttackRadius = 0.f;
				float DefendRadius = 0.f;

				// UINTERFACE 기반의 인터페이스 함수는 IBPI_EnemyAI::Execute_GetIdealRange()를 사용해야 함
				IBPI_EnemyAI::Execute_GetIdealRange(InPawn, AttackRadius, DefendRadius);

				// 해당 Float형 블랙보드키에 값을 저장
				if (BlackboardComp)
				{
					BlackboardComp->SetValueAsFloat(FName("AttackRadius"), AttackRadius);
					BlackboardComp->SetValueAsFloat(FName("DefendRadius"), DefendRadius);
				}
			}

			UE_LOG(LogTemp, Log, TEXT("Behavior Tree started successfully."));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to run Behavior Tree."));
		}
}


// 감지를 했는가? 
bool AAIC_EnemyBase::CanSenseActor(AActor* Actor, E_AISense Sense, FAIStimulus& OutStimulus)	// 매개변수 FAIStimulus& OutStimulus는 반환대상
{

	if (AIPerceptionComp && Actor)
	{
		// FAIPerceptionInfo 구조체에 Actor에 대한 감지 정보를 저장
		FActorPerceptionBlueprintInfo PerceptionInfo;

		//Actor에 대한 감지 정보가 있다면 PerceptionInfo를 채워주고 true를 반환
		if (AIPerceptionComp->GetActorsPerception(Actor, PerceptionInfo))			// 액터와 이 액터에 대해 인지하는 모든 감각에 대한 것을 얻음.
		{
			// 열거형을 UAISense 클래스로 변환 (사용자 정의함수. 현재 .h/.cpp 선언되어있음)
			TSubclassOf<UAISense> DesiredSenseClass = ConvertSenseToClass(Sense);
			if (DesiredSenseClass)
			{
				// 기본 Sense 객체의 Sense ID를 가져옵니다.
				UAISense* DefaultSense = DesiredSenseClass->GetDefaultObject<UAISense>();
				FAISenseID DesiredSenseID = DefaultSense->GetSenseID();

				// Last Sensed Stimuli 배열을 순회하며 검사
				for (const FAIStimulus& Stimulus : PerceptionInfo.LastSensedStimuli)
				{
					// Stimulus.Type이 원하는 Sense ID와 같고, 성공적으로 감지된 경우
					if (Stimulus.Type == DesiredSenseID && Stimulus.WasSuccessfullySensed())
					{
						// 발견된 Stimulus를 OutStimulus에 복사
						OutStimulus = Stimulus;
						return true;
					}
				}
			}
		}
	}

	return false;
}



// AI가 감각으로 무언가를 인식 할 때 함수가 발동, 매개변수로 인식한 액터 목록 제공
void AAIC_EnemyBase::OnPerceptionUpdate(const TArray<AActor*>& UpdatedActors)
{

	/*
		추가 기획:
		1. 감지된 대상의 Tag가 Player 또는 Craft인 경우 AttackTarget으로 지정한다.
		2. 현재 AttackTarget이 존재하지 않으면, 새 감지 대상(플레이어나 크래프트)을 바로 업데이트.
		3. 만약 현재 AttackTarget이 존재하면,
			- 그 대상이 Craft가 아니라면(예: Player), 새 대상이 있다면 업데이트하도록 처리 (거리 비교 등 추가 가능).
			- 그 대상이 Craft라면 기존 타겟을 유지한다.
	*/

	if (!BlackboardComp)
	{
		return;
	}

	// 현재 Blackboard에 저장된 AttackTarget을 가져옴
	AActor* CurrentAttackTarget = Cast<AActor>(BlackboardComp->GetValueAsObject(FName("AttackTarget")));
	FAIStimulus FoundStimulus;  // 디폴트 변수, 필요 시 각 루프 내에서 재사용

	// 새로 감지된 액터들을 순회하여, 태그가 "Player" 또는 "Craft"인 경우 처리
	for (AActor* SensedActor : UpdatedActors)
	{
		if (!SensedActor)
		{
			continue;
		}

		// SensedActor가 유효하고, 태그가 Player 또는 Craft이고, Sight로 감지되었으면 처리
		if ((SensedActor->ActorHasTag(TEXT("Player")) || SensedActor->ActorHasTag(TEXT("Craft")))
			&& CanSenseActor(SensedActor, E_AISense::Sight, FoundStimulus))
		{
			// 만약 현재 AttackTarget이 없다면 바로 새 대상으로 설정
			if (!CurrentAttackTarget)
			{
				HandleSensedSight(SensedActor);
				CurrentAttackTarget = SensedActor;  // 업데이트된 대상으로 갱신 (옵션)
			}
			else
			{
				// 현재 AttackTarget이 존재하면,
				// 만약 현재 타겟이 Craft이면 업데이트하지 않음
				if (CurrentAttackTarget->ActorHasTag(TEXT("Craft")))
				{
					UE_LOG(LogTemp, Log, TEXT("Current target (%s) is a Craft; new sensed actor (%s) ignored."),
						*CurrentAttackTarget->GetName(), *SensedActor->GetName());
					// 그냥 넘어감
				}
				else if (SensedActor != CurrentAttackTarget)
				{
					// 현재 타겟이 Craft가 아니면, 새 감지 대상로 업데이트 (조건에 따라 거리 비교 등 추가 가능)
					// 여기서는 단순히 새 대상이 다르면 업데이트하도록 처리함.
					
						UE_LOG(LogTemp, Log, TEXT("Updating AttackTarget from %s to %s."),
							*CurrentAttackTarget->GetName(), *SensedActor->GetName());
						HandleSensedSight(SensedActor);
						CurrentAttackTarget = SensedActor;  // 업데이트 (옵션)
				}
			}
		}
	}
}

// State 키를 패시브 상태로 전환
void AAIC_EnemyBase::SetStateAsPassive()
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsEnum(FName("State"), static_cast<uint8>(E_AIState::Passive));
	}
	
}

// State 키를 조사 중인 상태로 전환
void AAIC_EnemyBase::SetStateAsInvestigating(FVector& Location)
{
	// 블랙보드 컴포넌트를 가져오기
	
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsEnum(FName("State"), static_cast<uint8>(E_AIState::Investigating));	// 현재 상태를 조사중으로 전환
		BlackboardComp->SetValueAsVector(FName("PointOfInterest"), Location);	//  AI가 주목하거나 관심을 가져야 할 위치를 저장
	}
}

// State 키를 공격 상태로 전환
void AAIC_EnemyBase::SetStateAsAttacking(AActor* AttackTarget)
{

	// 플레이어 캐릭터의 값이 유효한지?

		if (BlackboardComp && AttackTarget)
		{
			// 블랙보드의 어택 타겟의 벨류 값을 PlayerCharacter로 지정
			BlackboardComp->SetValueAsObject(FName("AttackTarget"), AttackTarget);
			// 플레이어 상태 공격으로 전환
			BlackboardComp->SetValueAsEnum(FName("State"), static_cast<uint8>(E_AIState::Attacking));

			AttackTargetActor = AttackTarget;		// Attacking 시 __Context_AttackTarget에서 공격대상의 값을 가져올 예정

			// 만약 대상이 Craft나 Player 태그를 가지고 있다면,
			 // Destroy 시 Blackboard 값을 클리어하도록 OnDestroyed 이벤트에 바인딩합니다.
			if (AttackTarget->ActorHasTag(TEXT("Craft")) || AttackTarget->ActorHasTag(TEXT("Player")))
			{
				AttackTarget->OnDestroyed.RemoveDynamic(this, &AAIC_EnemyBase::OnAttackTargetDestroyed);
				AttackTarget->OnDestroyed.AddDynamic(this, &AAIC_EnemyBase::OnAttackTargetDestroyed);
			}
		}
}

// State 키를 Dead 상태로 전환
void AAIC_EnemyBase::SetStateAsDead()
{
	if (!IsValid(BlackboardComp)) return;

	BlackboardComp->SetValueAsEnum(FName("State"), static_cast<uint8>(E_AIState::Dead));
}


// E_AISense 열거형을 UAISense 클래스로 변환
TSubclassOf<class UAISense> AAIC_EnemyBase::ConvertSenseToClass(E_AISense Sense)
{
	switch (Sense)
	{
	case E_AISense::Sight:
		return UAISense_Sight::StaticClass();
	default:
		return nullptr;
	}
}


							//**      AI Perception      **//



// AI 시야에 걸렸다면...
void AAIC_EnemyBase::HandleSensedSight(AActor* AttackTarget)
{
	//if (AttackTarget)
	//{
	//	E_AIState EnumAIState = GetCurrentState();	// 현재 AI 상태를 반환받음
	//	
	//	// 현재 패시브 / 조사 중인 상태라면
	//	if (EnumAIState == E_AIState::Passive || EnumAIState == E_AIState::Investigating)
	//	{
	//		// 공격 실행
	//		SetStateAsAttacking(AttackTarget);
	//	}
	//}

	if (AttackTarget)
	{
		AActor* CurrentAttackTarget = Cast<AActor>(BlackboardComp->GetValueAsObject(FName("AttackTarget")));
		// 만약 현재 타겟이 없거나, 현재 타겟과 새 타겟이 다르면 공격 대상으로 설정
		if (!CurrentAttackTarget || CurrentAttackTarget != AttackTarget)
		{
			SetStateAsAttacking(AttackTarget);
		}
	}
}


							//**      AI Perception  END    **//
///////////////////////////////////////////////////////////////////////////////////////


AActor* AAIC_EnemyBase::GetAttackTargetActor()
{
	return AttackTargetActor;
}

void AAIC_EnemyBase::OnAttackTargetDestroyed(AActor* DestroyedActor)
{
	UE_LOG(LogTemp, Log, TEXT("AttackTarget %s destroyed, clearing Blackboard value."), *DestroyedActor->GetName());

	if (BlackboardComp)
	{
		BlackboardComp->ClearValue(FName("AttackTarget"));
	}

	// 현재 AI Perception 컴포넌트를 사용하여 Sight 감지된 액터들을 가져옵니다.
	TArray<AActor*> PerceivedActors;
	if (AIPerceptionComp)
	{
		AIPerceptionComp->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceivedActors);
	}

	// 새 공격 대상을 찾습니다.
	for (AActor* Actor : PerceivedActors)
	{
		if (Actor && (Actor->ActorHasTag(TEXT("Player")) || Actor->ActorHasTag(TEXT("Craft"))))
		{
			UE_LOG(LogTemp, Log, TEXT("New AttackTarget found: %s"), *Actor->GetName());
			SetStateAsAttacking(Actor);  // 새 공격 대상으로 전환
			return;
		}
	}

	// 만약 유효한 새 대상이 없다면, 상태를 Passive로 전환합니다.
	UE_LOG(LogTemp, Log, TEXT("No valid AttackTarget found, setting state to Passive."));
	SetStateAsPassive();
}

E_AIState AAIC_EnemyBase::GetCurrentState()
{
	if (BlackboardComp)
	{
		uint8 EnumValue = BlackboardComp->GetValueAsEnum(FName("State"));	// GetValueAsEnum은 Blackboard에 저장된 enum 값을 uint8 형태로 반환

		return static_cast<E_AIState>(EnumValue);	// 반환된 uint8 값을 E_AIState 열거형으로 변환하기 위해 static_cast를 사용
	}
	return E_AIState::Passive;	// 유효하지 않으면 기본상태 반환
}

