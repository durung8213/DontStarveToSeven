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
                if (CurrentPlayer->GetIntractionActorCanNextActive()) {     // ��ȣ �ۿ� �����ϸ�

                    FVector AddVector = CurrentPlayer->GetActorForwardVector();
                    // ������� Ȯ���ϴ� ĸ�� Ʈ���̽� ���!
                    FVector StartTrace = CurrentPlayer->WoodAxe1->GetComponentLocation();
                    FVector EndTrace = StartTrace + (AddVector * 125);;

                    TArray<AActor*> IgnoreActorArr;

                    TArray<TEnumAsByte<EObjectTypeQuery>> TraceObject;
                    EObjectTypeQuery ResourceObjectType = UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel8); // ���ҽ� ������Ʈ Ÿ��
                    EObjectTypeQuery WorldStaticObjectType = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_EngineTraceChannel1); // ���ҽ� ������Ʈ Ÿ��
                    TraceObject.Add(ResourceObjectType);
                    TraceObject.Add(WorldStaticObjectType);

                    TArray <FHitResult> HitResultArray;
                    FHitResult HitResult;

                    if (UKismetSystemLibrary::CapsuleTraceSingleForObjects(GetWorld(),      // ����� �˻� ���� �� Ʈ���̽���
                        StartTrace,
                        EndTrace,
                        5.f,
                        10.f,
                        TraceObject,
                        false,
                        IgnoreActorArr,
                        EDrawDebugTrace::None,   // Ʈ���̽� ����� 
                        HitResult,
                        true,
                        FLinearColor::Red,
                        FLinearColor::Green,
                        3.f)
                        )
                    {
                        CurrentPlayer->ServerMontagePlayWithSession(CurrentPlayer->InteractionMontage, CurrentPlayer->InteractionSectionName[1], 1.f);     // �� �κ� ġ��
                        return;
                    }

                    StartTrace = CurrentPlayer->WoodAxe2->GetComponentLocation();
                    EndTrace = StartTrace + (AddVector * 125);;

                    if (UKismetSystemLibrary::CapsuleTraceSingleForObjects(GetWorld(),      // �ߴ��� �˻����� �� Ʈ���̽��� ������
                        StartTrace,
                        EndTrace,
                        5.f,
                        10.f,
                        TraceObject,
                        false,
                        IgnoreActorArr,
                        EDrawDebugTrace::ForDuration,   // Ʈ���̽� ����� 
                        HitResult,
                        true,
                        FLinearColor::Red,
                        FLinearColor::Green,
                        3.f)
                        )
                    {
                        CurrentPlayer->ServerMontagePlayWithSession(CurrentPlayer->InteractionMontage, CurrentPlayer->InteractionSectionName[2], 1.f);     // �߰� �κ� ġ��
                        return;
                    }


                    CurrentPlayer->ServerMontagePlayWithSession(CurrentPlayer->InteractionMontage, CurrentPlayer->InteractionSectionName[3], 1.f);         // �ϴ� �κ� ġ��
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
