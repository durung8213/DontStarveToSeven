// Fill out your copyright notice in the Description page of Project Settings.


#include "Kimmh/Notify/DTS_WoodAxeSwingAnimNotify.h"
#include "Kimmh/BasePlayerCharacter.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

void UDTS_WoodAxeSwingAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner()) {
		ABasePlayerCharacter* CurrentPlayer = Cast<ABasePlayerCharacter>(MeshComp->GetOwner());
		if (CurrentPlayer) {
            if (CurrentPlayer->HasAuthority()) {
                if (CurrentPlayer->GetIntractionActorCanNextActive()) {     // 상호 작용 가능하면

                    FVector AddVector = CurrentPlayer->GetActorForwardVector();
                    // 상단인지 확인하는 캡슐 트레이스 출격!
                    FVector StartTrace = CurrentPlayer->WoodAxe1->GetComponentLocation();
                    FVector EndTrace = StartTrace + (AddVector * 125);;

                    TArray<AActor*> IgnoreActorArr;

                    TArray<TEnumAsByte<EObjectTypeQuery>> TraceObject;
                    EObjectTypeQuery ResourceObjectType = UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel8); // 리소스 오브젝트 타입
                    EObjectTypeQuery WorldStaticObjectType = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_EngineTraceChannel1); // 리소스 오브젝트 타입
                    TraceObject.Add(ResourceObjectType);
                    TraceObject.Add(WorldStaticObjectType);

                    TArray <FHitResult> HitResultArray;
                    FHitResult HitResult;

                    if (UKismetSystemLibrary::CapsuleTraceSingleForObjects(GetWorld(),      // 상단을 검사 했을 때 트레이스에
                        StartTrace,
                        EndTrace,
                        5.f,
                        10.f,
                        TraceObject,
                        false,
                        IgnoreActorArr,
                        EDrawDebugTrace::None,   // 트레이스 디버그 
                        HitResult,
                        true,
                        FLinearColor::Red,
                        FLinearColor::Green,
                        3.f)
                        )
                    {
                        CurrentPlayer->ServerMontagePlayWithSession(CurrentPlayer->InteractionMontage, CurrentPlayer->InteractionSectionName[1], 1.f);     // 윗 부분 치기
                        return;
                    }

                    StartTrace = CurrentPlayer->WoodAxe2->GetComponentLocation();
                    EndTrace = StartTrace + (AddVector * 125);;

                    if (UKismetSystemLibrary::CapsuleTraceSingleForObjects(GetWorld(),      // 중단을 검사했을 때 트레이스에 잡히면
                        StartTrace,
                        EndTrace,
                        5.f,
                        10.f,
                        TraceObject,
                        false,
                        IgnoreActorArr,
                        EDrawDebugTrace::ForDuration,   // 트레이스 디버그 
                        HitResult,
                        true,
                        FLinearColor::Red,
                        FLinearColor::Green,
                        3.f)
                        )
                    {
                        CurrentPlayer->ServerMontagePlayWithSession(CurrentPlayer->InteractionMontage, CurrentPlayer->InteractionSectionName[2], 1.f);     // 중간 부분 치기
                        return;
                    }


                    CurrentPlayer->ServerMontagePlayWithSession(CurrentPlayer->InteractionMontage, CurrentPlayer->InteractionSectionName[3], 1.f);         // 하단 부분 치기
                }
                else {
                    //CurrentPlayer->ServerMontagePlayWithSession(CurrentPlayer->InteractionMontage, CurrentPlayer->InteractionSectionName[8]);
                    CurrentPlayer->ServerSetToolCollisionEnable(ETool::WoodAxe, false);
                    CurrentPlayer->MultiSetToolVisibility(ETool::WoodAxe, false);
                }
            }
		}
	}
}
