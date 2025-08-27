// Fill out your copyright notice in the Description page of Project Settings.


#include "Hamkt/Enemy/AI/AIC_EnemyBase.h"
#include "GameFramework/Pawn.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"              // AActor ����
#include "TimerManager.h"					  // SetTimer �Լ�
#include "Engine/Engine.h"                    // UE_LOG ���
#include "Hamkt/Enemy/AI/BPI_EnemyAI.h"
#include "Hamkt/Enemy/EnemyBase.h"

#include "Hamkt/Enemy/AI/Enums/E_AIState.h"
#include "Perception/AISense.h"                // FAIPerceptionInfo, FAIStimulus ���� (�ʿ信 ����)
#include "Perception/AIPerceptionTypes.h"		// FAIPerceptionInfo ����
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"



AAIC_EnemyBase::AAIC_EnemyBase()
{
	//////////////////////////////////////////////////////////////////////////////
	// 
	// AI Perception Component ����
	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));

	// �þ�, û��, ������ ������ ���� SenseConfig ����
	SightConfig	  =	CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	

	// SightConfig �Ӽ� ����
	SightConfig->SightRadius = 800.f;								// AI�� �þ߸� ���� ���� �� �� �ִ� �ִ� �Ÿ�
	SightConfig->LoseSightRadius = 1200.f;							// ������ ��ü�� �ش� ������ ����� �� �̻� �ð������� �ν����� ����... 
																		// �׷��Ƿ� SightRadius ���� ū ���̾����.						
	SightConfig->PeripheralVisionAngleDegrees = 80.f;				// AI�� �þ߰� (���ʾ�)	
	SightConfig->SetMaxAge(5.f);									// ������ �ڱ��� �޸𸮿� �����ִ� �ִ� �ð�(��)

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;		// �������� �з��� ��ü�� �����Ұ���?
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;	// �Ʊ����� �з��� ��ü�� �����Ұ���?
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;		// �߸����� �з��� ��ü�� �����Ұ���?

	// Perception Component�� �þ�, û��, ������ ���� ���� �߰�
	AIPerceptionComp->ConfigureSense(*SightConfig);

	// Dominant Sense�� �ð� ������ ���� (AI�� �켱������ �ð� ������ ���)
	AIPerceptionComp->SetDominantSense(UAISense_Sight::StaticClass());


	//////////////////////////////////////////////////////////////////////////////

	// OnPerceptionUpdate �̺�Ʈ�� �Լ� ���ε� (���� ��Ƽĳ��Ʈ ��������Ʈ)
	if (AIPerceptionComp)
	{
		// �ߺ� ��ϵǾ� ���� ���� ���� �߰�
		AIPerceptionComp->OnPerceptionUpdated.RemoveDynamic(this, &AAIC_EnemyBase::OnPerceptionUpdate);
		AIPerceptionComp->OnPerceptionUpdated.AddDynamic(this, &AAIC_EnemyBase::OnPerceptionUpdate);
	}
}

void AAIC_EnemyBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	UE_LOG(LogTemp, Log, TEXT("AAIC_EnemyBase has possessed pawn : %s"), *InPawn->GetName());

	// ���� �����̺��Ʈ�� ���� ��ȿ����?
	if (!IsValid(BehaviorTreeAsset)) return;

	if (!IsValid(InPawn)) return;

	AEnemyBase* EnemyBase = Cast<AEnemyBase>(InPawn);	// ���� Pawn�� EnemyBase�� ����ȯ
	if (!IsValid(EnemyBase)) return;

	if (!IsValid(EnemyBase->GetBehaviorTree())) return;	// �����̺��Ʈ�� �� �ִ��� Ȯ��
	
	
		// �������̺��� �������Ʈ���� ��ȯ���� ����.. �׷��� if������ �۾���.
		if (RunBehaviorTree(EnemyBase->GetBehaviorTree()))
		{
			BlackboardComp = GetBlackboardComponent();		// ���� ������ ������Ʈ ��������

			// ���¸� Passive�� ����
			SetStateAsPassive();

			if (InPawn->Implements<UBPI_EnemyAI>())
			{
				// ���� ��Ÿ� / ��� ������ ������ ���� ����
				float AttackRadius = 0.f;
				float DefendRadius = 0.f;

				// UINTERFACE ����� �������̽� �Լ��� IBPI_EnemyAI::Execute_GetIdealRange()�� ����ؾ� ��
				IBPI_EnemyAI::Execute_GetIdealRange(InPawn, AttackRadius, DefendRadius);

				// �ش� Float�� ������Ű�� ���� ����
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


// ������ �ߴ°�? 
bool AAIC_EnemyBase::CanSenseActor(AActor* Actor, E_AISense Sense, FAIStimulus& OutStimulus)	// �Ű����� FAIStimulus& OutStimulus�� ��ȯ���
{

	if (AIPerceptionComp && Actor)
	{
		// FAIPerceptionInfo ����ü�� Actor�� ���� ���� ������ ����
		FActorPerceptionBlueprintInfo PerceptionInfo;

		//Actor�� ���� ���� ������ �ִٸ� PerceptionInfo�� ä���ְ� true�� ��ȯ
		if (AIPerceptionComp->GetActorsPerception(Actor, PerceptionInfo))			// ���Ϳ� �� ���Ϳ� ���� �����ϴ� ��� ������ ���� ���� ����.
		{
			// �������� UAISense Ŭ������ ��ȯ (����� �����Լ�. ���� .h/.cpp ����Ǿ�����)
			TSubclassOf<UAISense> DesiredSenseClass = ConvertSenseToClass(Sense);
			if (DesiredSenseClass)
			{
				// �⺻ Sense ��ü�� Sense ID�� �����ɴϴ�.
				UAISense* DefaultSense = DesiredSenseClass->GetDefaultObject<UAISense>();
				FAISenseID DesiredSenseID = DefaultSense->GetSenseID();

				// Last Sensed Stimuli �迭�� ��ȸ�ϸ� �˻�
				for (const FAIStimulus& Stimulus : PerceptionInfo.LastSensedStimuli)
				{
					// Stimulus.Type�� ���ϴ� Sense ID�� ����, ���������� ������ ���
					if (Stimulus.Type == DesiredSenseID && Stimulus.WasSuccessfullySensed())
					{
						// �߰ߵ� Stimulus�� OutStimulus�� ����
						OutStimulus = Stimulus;
						return true;
					}
				}
			}
		}
	}

	return false;
}



// AI�� �������� ���𰡸� �ν� �� �� �Լ��� �ߵ�, �Ű������� �ν��� ���� ��� ����
void AAIC_EnemyBase::OnPerceptionUpdate(const TArray<AActor*>& UpdatedActors)
{

	/*
		�߰� ��ȹ:
		1. ������ ����� Tag�� Player �Ǵ� Craft�� ��� AttackTarget���� �����Ѵ�.
		2. ���� AttackTarget�� �������� ������, �� ���� ���(�÷��̾ ũ����Ʈ)�� �ٷ� ������Ʈ.
		3. ���� ���� AttackTarget�� �����ϸ�,
			- �� ����� Craft�� �ƴ϶��(��: Player), �� ����� �ִٸ� ������Ʈ�ϵ��� ó�� (�Ÿ� �� �� �߰� ����).
			- �� ����� Craft��� ���� Ÿ���� �����Ѵ�.
	*/

	if (!BlackboardComp)
	{
		return;
	}

	// ���� Blackboard�� ����� AttackTarget�� ������
	AActor* CurrentAttackTarget = Cast<AActor>(BlackboardComp->GetValueAsObject(FName("AttackTarget")));
	FAIStimulus FoundStimulus;  // ����Ʈ ����, �ʿ� �� �� ���� ������ ����

	// ���� ������ ���͵��� ��ȸ�Ͽ�, �±װ� "Player" �Ǵ� "Craft"�� ��� ó��
	for (AActor* SensedActor : UpdatedActors)
	{
		if (!SensedActor)
		{
			continue;
		}

		// SensedActor�� ��ȿ�ϰ�, �±װ� Player �Ǵ� Craft�̰�, Sight�� �����Ǿ����� ó��
		if ((SensedActor->ActorHasTag(TEXT("Player")) || SensedActor->ActorHasTag(TEXT("Craft")))
			&& CanSenseActor(SensedActor, E_AISense::Sight, FoundStimulus))
		{
			// ���� ���� AttackTarget�� ���ٸ� �ٷ� �� ������� ����
			if (!CurrentAttackTarget)
			{
				HandleSensedSight(SensedActor);
				CurrentAttackTarget = SensedActor;  // ������Ʈ�� ������� ���� (�ɼ�)
			}
			else
			{
				// ���� AttackTarget�� �����ϸ�,
				// ���� ���� Ÿ���� Craft�̸� ������Ʈ���� ����
				if (CurrentAttackTarget->ActorHasTag(TEXT("Craft")))
				{
					UE_LOG(LogTemp, Log, TEXT("Current target (%s) is a Craft; new sensed actor (%s) ignored."),
						*CurrentAttackTarget->GetName(), *SensedActor->GetName());
					// �׳� �Ѿ
				}
				else if (SensedActor != CurrentAttackTarget)
				{
					// ���� Ÿ���� Craft�� �ƴϸ�, �� ���� ���� ������Ʈ (���ǿ� ���� �Ÿ� �� �� �߰� ����)
					// ���⼭�� �ܼ��� �� ����� �ٸ��� ������Ʈ�ϵ��� ó����.
					
						UE_LOG(LogTemp, Log, TEXT("Updating AttackTarget from %s to %s."),
							*CurrentAttackTarget->GetName(), *SensedActor->GetName());
						HandleSensedSight(SensedActor);
						CurrentAttackTarget = SensedActor;  // ������Ʈ (�ɼ�)
				}
			}
		}
	}
}

// State Ű�� �нú� ���·� ��ȯ
void AAIC_EnemyBase::SetStateAsPassive()
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsEnum(FName("State"), static_cast<uint8>(E_AIState::Passive));
	}
	
}

// State Ű�� ���� ���� ���·� ��ȯ
void AAIC_EnemyBase::SetStateAsInvestigating(FVector& Location)
{
	// ������ ������Ʈ�� ��������
	
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsEnum(FName("State"), static_cast<uint8>(E_AIState::Investigating));	// ���� ���¸� ���������� ��ȯ
		BlackboardComp->SetValueAsVector(FName("PointOfInterest"), Location);	//  AI�� �ָ��ϰų� ������ ������ �� ��ġ�� ����
	}
}

// State Ű�� ���� ���·� ��ȯ
void AAIC_EnemyBase::SetStateAsAttacking(AActor* AttackTarget)
{

	// �÷��̾� ĳ������ ���� ��ȿ����?

		if (BlackboardComp && AttackTarget)
		{
			// �������� ���� Ÿ���� ���� ���� PlayerCharacter�� ����
			BlackboardComp->SetValueAsObject(FName("AttackTarget"), AttackTarget);
			// �÷��̾� ���� �������� ��ȯ
			BlackboardComp->SetValueAsEnum(FName("State"), static_cast<uint8>(E_AIState::Attacking));

			AttackTargetActor = AttackTarget;		// Attacking �� __Context_AttackTarget���� ���ݴ���� ���� ������ ����

			// ���� ����� Craft�� Player �±׸� ������ �ִٸ�,
			 // Destroy �� Blackboard ���� Ŭ�����ϵ��� OnDestroyed �̺�Ʈ�� ���ε��մϴ�.
			if (AttackTarget->ActorHasTag(TEXT("Craft")) || AttackTarget->ActorHasTag(TEXT("Player")))
			{
				AttackTarget->OnDestroyed.RemoveDynamic(this, &AAIC_EnemyBase::OnAttackTargetDestroyed);
				AttackTarget->OnDestroyed.AddDynamic(this, &AAIC_EnemyBase::OnAttackTargetDestroyed);
			}
		}
}

// State Ű�� Dead ���·� ��ȯ
void AAIC_EnemyBase::SetStateAsDead()
{
	if (!IsValid(BlackboardComp)) return;

	BlackboardComp->SetValueAsEnum(FName("State"), static_cast<uint8>(E_AIState::Dead));
}


// E_AISense �������� UAISense Ŭ������ ��ȯ
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



// AI �þ߿� �ɷȴٸ�...
void AAIC_EnemyBase::HandleSensedSight(AActor* AttackTarget)
{
	//if (AttackTarget)
	//{
	//	E_AIState EnumAIState = GetCurrentState();	// ���� AI ���¸� ��ȯ����
	//	
	//	// ���� �нú� / ���� ���� ���¶��
	//	if (EnumAIState == E_AIState::Passive || EnumAIState == E_AIState::Investigating)
	//	{
	//		// ���� ����
	//		SetStateAsAttacking(AttackTarget);
	//	}
	//}

	if (AttackTarget)
	{
		AActor* CurrentAttackTarget = Cast<AActor>(BlackboardComp->GetValueAsObject(FName("AttackTarget")));
		// ���� ���� Ÿ���� ���ų�, ���� Ÿ�ٰ� �� Ÿ���� �ٸ��� ���� ������� ����
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

	// ���� AI Perception ������Ʈ�� ����Ͽ� Sight ������ ���͵��� �����ɴϴ�.
	TArray<AActor*> PerceivedActors;
	if (AIPerceptionComp)
	{
		AIPerceptionComp->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceivedActors);
	}

	// �� ���� ����� ã���ϴ�.
	for (AActor* Actor : PerceivedActors)
	{
		if (Actor && (Actor->ActorHasTag(TEXT("Player")) || Actor->ActorHasTag(TEXT("Craft"))))
		{
			UE_LOG(LogTemp, Log, TEXT("New AttackTarget found: %s"), *Actor->GetName());
			SetStateAsAttacking(Actor);  // �� ���� ������� ��ȯ
			return;
		}
	}

	// ���� ��ȿ�� �� ����� ���ٸ�, ���¸� Passive�� ��ȯ�մϴ�.
	UE_LOG(LogTemp, Log, TEXT("No valid AttackTarget found, setting state to Passive."));
	SetStateAsPassive();
}

E_AIState AAIC_EnemyBase::GetCurrentState()
{
	if (BlackboardComp)
	{
		uint8 EnumValue = BlackboardComp->GetValueAsEnum(FName("State"));	// GetValueAsEnum�� Blackboard�� ����� enum ���� uint8 ���·� ��ȯ

		return static_cast<E_AIState>(EnumValue);	// ��ȯ�� uint8 ���� E_AIState ���������� ��ȯ�ϱ� ���� static_cast�� ���
	}
	return E_AIState::Passive;	// ��ȿ���� ������ �⺻���� ��ȯ
}

