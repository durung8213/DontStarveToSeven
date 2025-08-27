// Fill out your copyright notice in the Description page of Project Settings.


#include "Hamkt/Enemy/AI/Services/BTS_UpdateFocusDuringAttack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"
#include "Kismet/KismetMathLibrary.h"


UBTS_UpdateFocusDuringAttack::UBTS_UpdateFocusDuringAttack()
{
	// 자주 업데이트 하도록 짧은 간격
	Interval = 0.2f;
	
	// TickNode를 호출
	bNotifyTick = true;
	NodeName = TEXT("Update Focus During Attack");
}

void UBTS_UpdateFocusDuringAttack::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!IsValid(AIController)) return;

	APawn* AIPawn = AIController->GetPawn();
	if (!IsValid(AIPawn)) return;

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!IsValid(BlackboardComp)) return;

	AActor* AttackTarget = Cast<AActor>(BlackboardComp->GetValueAsObject(AttackTargetKey.SelectedKeyName));
	if (!IsValid(AttackTarget)) return;

	// 현재 pawn 위치와 타깃 위치로부터 lookat 회전 계산
	FVector AILocation = AIPawn->GetActorLocation();
	FVector TargetLocation = AttackTarget->GetActorLocation();
	FRotator DisiredRot = UKismetMathLibrary::FindLookAtRotation(AILocation, TargetLocation);

	// 현재 컨트롤러 회전과 DesiredRot을 보간(interp)하여 부드럽게 전환
	FRotator CurrentRot = AIController->GetControlRotation();
	FRotator NewRot = FMath::RInterpTo(CurrentRot, DisiredRot, DeltaSeconds, RotationInterpSpeed);

	// 업데이트된 회전값을 컨트롤러에 적용 (캐릭터가 이를 따라 회전해야함)
	AIController->SetControlRotation(NewRot);


}
