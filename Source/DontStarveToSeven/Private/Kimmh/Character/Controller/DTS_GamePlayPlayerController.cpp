// Fill out your copyright notice in the Description page of Project Settings.


#include "Kimmh/Character/Controller/DTS_GamePlayPlayerController.h"
#include "Kimmh/BasePlayerCharacter.h"
#include "Kimmh/HUDnUI/HUD/DTS_BaseGamePlayHUD.h"
#include "Kimmh/HUDnUI/UI/DTS_BaseInteractionUserWidget.h"
#include "Kimmh/Interface/DTS_BPIInteraction.h" 
#include "Kimmh/Character/PlayerState/DTS_BasePlayerState.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Engine/AssetManager.h"
#include "Net/UnrealNetwork.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Seojy/CraftManager.h"
#include "Leesh/SHInventoryComponent.h"
#include "Hamkt/Enemy/EnemyBoss/EnemyBossBase.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SphereComponent.h"
#include "Hamkt/Enemy/EnemyBase.h"
#include "Leesh/SHGameModeBase.h"
#include "Leesh/GameInstance/DTS_GameInstance.h"

void ADTS_GamePlayPlayerController::ServerRemoveItem_Implementation(const FInventory& Item, EInventoryType InventoryType)
{
    InventoryComponent->RemoveItem(Item, InventoryType);
}

int ADTS_GamePlayPlayerController::SearchItemCountToInventory(FString InItemName)
{
    return InventoryComponent->SearchItemCount(InItemName, EInventoryType::Inventory);
}

int ADTS_GamePlayPlayerController::SearchItemCountToQuickSlot(FString InItemName)
{
    return InventoryComponent->SearchItemCount(InItemName, EInventoryType::QuickSlot);
}

void ADTS_GamePlayPlayerController::AddItemToInventoryByName_Implementation(const FString& ItemName, int Count)
{
    InventoryComponent->AddItem(FInventory(false, FCoordinate(false, 0, 0), ItemName, Count), EInventoryType::Inventory);
}


void ADTS_GamePlayPlayerController::RemoveItemToInventoryByName_Implementation(const FString& ItemName, int Count)
{
    InventoryComponent->RemoveItem(FInventory(false, FCoordinate(false, 0, 0), ItemName, Count), EInventoryType::Inventory);
}

void ADTS_GamePlayPlayerController::BeginPlay()
{
    Super::BeginPlay();  
    bAutoManageActiveCameraTarget = false;

    FTimerHandle TimerHandle;

    GetWorld()->GetTimerManager().SetTimer(
        TimerHandle,
        this,
        &ADTS_GamePlayPlayerController::FindNormalEnemy,
        0.1f,
        true
    );
}

void ADTS_GamePlayPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    FindInteractionActor();
    if (bAutoMove) {
        ABasePlayerCharacter* CurrentPlayerCharacter = Cast<ABasePlayerCharacter>(GetPawn());
        ServerNavMove();
        CurrentPlayerCharacter->AddMovementInput(NewDir, 1.0f);

        if (!InteractionActor || FVector::Dist(GetPawn()->GetActorLocation(), InteractionActor->GetActorLocation()) <= 30.f) {
            bAutoMove = false;
        }
    }
}

/// <summary>
/// 인풋 함수를 바인딩(초기 바인딩 설정) 하는 함수
/// </summary>
void ADTS_GamePlayPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    // Enhanced Input 서브시스템 가져오기
    if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
        ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        InputSubsystem->AddMappingContext(DefaultInputMappingContext, 0);
        CurrentMappingContext = DefaultInputMappingContext;
    }

    UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent);
    if (EnhancedInput)
    {
        EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADTS_GamePlayPlayerController::MoveInput);   // 기본 움직임 액션 바인딩

        EnhancedInput->BindAction(CameraAction, ETriggerEvent::Triggered, this, &ADTS_GamePlayPlayerController::CameraInput);   // 기본 카메라 움직임 액션 바인딩

        EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ADTS_GamePlayPlayerController::JumpKeyStarted);    // 기본 점프 누름 액션 바인딩
        EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &ADTS_GamePlayPlayerController::JumpKeyCompleted);    // 기본 점프 버튼 땜 액션 바인딩

        EnhancedInput->BindAction(OpenInventoryUIAction, ETriggerEvent::Started, this, &ADTS_GamePlayPlayerController::OpenInventoryUI);    // 인벤토리 UI 오픈 액션 바인딩

        EnhancedInput->BindAction(OpenCreaftingUIAction, ETriggerEvent::Started, this, &ADTS_GamePlayPlayerController::OpenCreaftingUI);    // 크래프팅 UI 오픈 액션 바인딩

        EnhancedInput->BindAction(CloseInventoryAction, ETriggerEvent::Started, this, &ADTS_GamePlayPlayerController::CloseInventoryUI);    // 인벤토리 UI 닫는 액션 바인딩

        EnhancedInput->BindAction(CloseCreaftingAction, ETriggerEvent::Started, this, &ADTS_GamePlayPlayerController::CloseCreaftingUI);    // 크래프팅 UI 닫는 액션 바인딩

        EnhancedInput->BindAction(QuickSlot1Action, ETriggerEvent::Started, this, &ADTS_GamePlayPlayerController::QuickSlot1);    // 크래프팅 UI 닫는 액션 바인딩
        EnhancedInput->BindAction(QuickSlot2Action, ETriggerEvent::Started, this, &ADTS_GamePlayPlayerController::QuickSlot2);    // 크래프팅 UI 닫는 액션 바인딩
        EnhancedInput->BindAction(QuickSlot3Action, ETriggerEvent::Started, this, &ADTS_GamePlayPlayerController::QuickSlot3);    // 크래프팅 UI 닫는 액션 바인딩
        EnhancedInput->BindAction(QuickSlot4Action, ETriggerEvent::Started, this, &ADTS_GamePlayPlayerController::QuickSlot4);    // 크래프팅 UI 닫는 액션 바인딩
        EnhancedInput->BindAction(QuickSlot5Action, ETriggerEvent::Started, this, &ADTS_GamePlayPlayerController::QuickSlot5);    // 크래프팅 UI 닫는 액션 바인딩
        
        EnhancedInput->BindAction(InteractionAction, ETriggerEvent::Started, this, &ADTS_GamePlayPlayerController::Interaction);    // 상호 작용 바인딩
        

        EnhancedInput->BindAction(AttackAction, ETriggerEvent::Started, this, &ADTS_GamePlayPlayerController::Attack);              // 일반 공격 액션 바인딩
        EnhancedInput->BindAction(StrongAttackAction, ETriggerEvent::Started, this, &ADTS_GamePlayPlayerController::StrongAttack);  // 강한 공격 액션 바인딩

        EnhancedInput->BindAction(AimingAction, ETriggerEvent::Ongoing, this, &ADTS_GamePlayPlayerController::BowAiming);  // 활 에이밍 액션 바인딩
        EnhancedInput->BindAction(ShotAction, ETriggerEvent::Completed, this, &ADTS_GamePlayPlayerController::BowShot);  // 활 쏘기 액션 바인딩
        EnhancedInput->BindAction(AimingReleasedAction, ETriggerEvent::Started, this, &ADTS_GamePlayPlayerController::BowAimReleased);  // 활 에이밍 취소 액션 바인딩

        EnhancedInput->BindAction(CreateAction, ETriggerEvent::Started, this, &ADTS_GamePlayPlayerController::CreateStructure);     // 제작 액션 바인딩
        EnhancedInput->BindAction(CancleCreaftingModeAction, ETriggerEvent::Started, this, &ADTS_GamePlayPlayerController::CancleCraftingMode); // 제작 모드 취소 액션 바인딩
        EnhancedInput->BindAction(RotateLeft, ETriggerEvent::Started, this, &ADTS_GamePlayPlayerController::StructureTurnLeft);     // 제작모드 구조물 반시계 방향 회전 바인딩
        EnhancedInput->BindAction(RotateRight, ETriggerEvent::Started, this, &ADTS_GamePlayPlayerController::StructureTurnRight);   // 제작 모드 구조물 시계 방향 회전 바인딩
    }
    else {
        UE_LOG(LogTemp, Log, TEXT("EnhancedInput 캐스트 실패"));
    }
}


ADTS_GamePlayPlayerController::ADTS_GamePlayPlayerController()
{
    InventoryComponent = CreateDefaultSubobject<USHInventoryComponent>(TEXT("Inventory"));
    QuickSlotInventoryComponent = CreateDefaultSubobject<USHInventoryComponent>(TEXT("QuickSlotInventory"));
}

void ADTS_GamePlayPlayerController::OnNetCleanup(UNetConnection* Connection)
{
    // 저장 로직 필요
    ASHGameModeBase* CurrentGameMode = Cast<ASHGameModeBase>(GetWorld()->GetAuthGameMode());
    if (CurrentGameMode) {
        // 게임모드가 SHGameMode가 맞으면
        CurrentGameMode->UpdatePlayerInfo(NickName, GetPlayerStat(), InventoryComponent->Inventory, InventoryComponent->QuickSlot);
    }


    Super::OnNetCleanup(Connection);
}

/*            LeeSH 제작 저장 관련             */
void ADTS_GamePlayPlayerController::ClientRequestNickname_Implementation()
{
    GetNickName();
}

void ADTS_GamePlayPlayerController::GetNickName()
{
    // GameInstance에서 닉네임 가져오기
    if (UDTS_GameInstance* GI = Cast<UDTS_GameInstance>(GetGameInstance()))
    {
    	NickName = GI->NickName;
    	ServerSetNickName(NickName); // 서버에서 Replicate 되도록 설정
    }

}


bool ADTS_GamePlayPlayerController::ServerSetNickName_Validate(const FString& NewNickName)
{
    return true;
}

void ADTS_GamePlayPlayerController::ServerSetNickName_Implementation(const FString& NewNickName)
{
    NickName = NewNickName;
}


bool ADTS_GamePlayPlayerController::ServerTossInfoToGameMode_Validate()
{
    return true;
}

void ADTS_GamePlayPlayerController::ServerTossInfoToGameMode_Implementation()
{
    ASHGameModeBase* GM = Cast<ASHGameModeBase>(GetWorld()->GetAuthGameMode());
    if (GM)
    {
        ADTS_BasePlayerState* State = Cast<ADTS_BasePlayerState>(PlayerState);
        if (State)
        {
            UE_LOG(LogTemp, Warning, TEXT("Success to cast GM"));
            FCharacterStat TempStatStruct = State->GetAllPlayerStat();
            GM->UpdatePlayerInfo(NickName, TempStatStruct, InventoryComponent->Inventory, InventoryComponent->QuickSlot);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Fail to cast GM"));
        }
    }


}

/*            LeeSH 제작 저장 관련             */

/// <summary>
/// IMC를 변경하는 함수
/// </summary>
/// <param name="IMC">IMC에 부여된 ENUM</param>
void ADTS_GamePlayPlayerController::ChangeInputMappingContext(EIMC IMC)
{
    if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
        ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        
        InputSubsystem->RemoveMappingContext(CurrentMappingContext);

        if (InputSubsystem->HasMappingContext(DefaultInputMappingContext)) {
            InputSubsystem->RemoveMappingContext(DefaultInputMappingContext);
        }
        if (InputSubsystem->HasMappingContext(InventoryInputMappingContext)) {
            InputSubsystem->RemoveMappingContext(InventoryInputMappingContext);
        }
        if (InputSubsystem->HasMappingContext(CreaftingInputMappingContext)) {
            InputSubsystem->RemoveMappingContext(CreaftingInputMappingContext);
        }
        if (InputSubsystem->HasMappingContext(CreaftingModeInputMappingContext)) {
            InputSubsystem->RemoveMappingContext(CreaftingModeInputMappingContext);
        }
        if (InputSubsystem->HasMappingContext(BowInputMappingContext)) {
            InputSubsystem->RemoveMappingContext(BowInputMappingContext);
        }

        FString Message;
        FTimerHandle IMCRemoveTimerHandle;
        UInputMappingContext* NewMappingContext = nullptr;
        
        switch (IMC)
        {
        case EIMC::DefaultIMC:
            //RebindActions();
            InputSubsystem->AddMappingContext(DefaultInputMappingContext, 0);
            CurrentMappingContext = DefaultInputMappingContext;
            //NewMappingContext = DefaultInputMappingContext;
            
            this->SetInputMode(FInputModeGameOnly());
            this->SetShowMouseCursor(false);
            
            Message = FString::Printf(TEXT("DefaultIMC"));
            GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, Message);
            
            break;
        case EIMC::InventoryUIIMC:
            //RebindActions();
            InputSubsystem->AddMappingContext(InventoryInputMappingContext, 0);
            CurrentMappingContext = InventoryInputMappingContext;
            
            //NewMappingContext = InventoryInputMappingContext;

            this->SetInputMode(FInputModeGameAndUI());
            this->SetShowMouseCursor(true);
            
            Message = FString::Printf(TEXT("InventoryUIIMC"));
            GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, Message);
            
            break;
        case EIMC::CreaftingUIIMC:
            //RebindActions();
            InputSubsystem->AddMappingContext(CreaftingInputMappingContext, 0);
            CurrentMappingContext = CreaftingInputMappingContext;
            
            //NewMappingContext = CreaftingInputMappingContext;

            this->SetInputMode(FInputModeGameAndUI());
            this->SetShowMouseCursor(true);
            
            Message = FString::Printf(TEXT("CreaftingUIIMC"));
            GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, Message);
            
            break;
        case EIMC::CreaftingIMC:
            InputSubsystem->AddMappingContext(CreaftingModeInputMappingContext, 0);
            if (GetHUD()) {
                ADTS_BaseGamePlayHUD* BaseGamePlayHUD = Cast<ADTS_BaseGamePlayHUD>(GetHUD());
                if (BaseGamePlayHUD)
                {
                    BaseGamePlayHUD->CloseCreaftingUI();
                }
                else {
                    Message = FString::Printf(TEXT("BaseGamePlayHUD Cast Fail"));
                    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, Message); // 화면에 표시
                }
            }
            else {
                Message = FString::Printf(TEXT("Call Hud Fail"));
                GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, Message); // 화면에 표시
            }
            this->SetInputMode(FInputModeGameOnly());
            this->SetShowMouseCursor(false);
            break;
        case EIMC::BowIMC:
            InputSubsystem->AddMappingContext(BowInputMappingContext, 0);
            CurrentMappingContext = BowInputMappingContext;
            //NewMappingContext = DefaultInputMappingContext;

            this->SetInputMode(FInputModeGameOnly());
            this->SetShowMouseCursor(false);

            Message = FString::Printf(TEXT("BowIMC"));
            GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, Message);
            break;
        default:
            break;
        }
    }
}

void ADTS_GamePlayPlayerController::FindNormalEnemy_Implementation()
{
    FVector StartTrace;
    FVector EndTrace;
    FRotator PlayerViewRot;

    GetPlayerViewPoint(StartTrace, PlayerViewRot);
    FVector AddVector = UKismetMathLibrary::GetForwardVector(PlayerViewRot);

    StartTrace = StartTrace + (AddVector * 500);

    EndTrace = StartTrace + (AddVector * 800);

    TArray<AActor*> IgnoreActorArr;

    if (GetCurrentPlayerCharacter()) {
        if (GetCurrentPlayerCharacter()->CurrentEquipItem) {
            IgnoreActorArr.Add(GetCurrentPlayerCharacter()->CurrentEquipItem);
            
        }
        IgnoreActorArr.Add(GetCurrentPlayerCharacter());
    }

    TArray<TEnumAsByte<EObjectTypeQuery>> TraceObject;
    // "Player"라는 오브젝트 채널을 찾아서 변환 후 추가
    EObjectTypeQuery EnemyObjectType = UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel3); // 적절한 채널 번호 사용
    TraceObject.Add(EnemyObjectType);

    TArray <FHitResult> HitResultArray;
    FHitResult HitResult;

    if (UKismetSystemLibrary::BoxTraceMultiForObjects(
        GetWorld(),
        StartTrace,
        StartTrace,
        FVector(200, 200, 200),
        FRotator(0, 0, 0),
        TraceObject,
        false,
        IgnoreActorArr,
        EDrawDebugTrace::ForDuration,
        HitResultArray,
        true,
        FLinearColor::Red,
        FLinearColor::Green,
        1.f )) 
    {
        for (FHitResult hit : HitResultArray) {
            AEnemyBase* CurrentEnemy = Cast<AEnemyBase>(hit.GetActor());
            if (CurrentEnemy) {
                // 보이게 하는 함수
                CurrentEnemy->ShowHealthBarForDuration(2.f);
            }
        }
    }
}

/// <summary>
/// 폐기 예정
/// </summary>
void ADTS_GamePlayPlayerController::RebindActions()
{
    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
    {
        // 기존 바인딩 제거
        EnhancedInput->ClearActionBindings();

        // 현재 IMC에 따라 바인딩
        if (CurrentMappingContext == DefaultInputMappingContext)
        {
            EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADTS_GamePlayPlayerController::MoveInput);
            EnhancedInput->BindAction(CameraAction, ETriggerEvent::Triggered, this, &ADTS_GamePlayPlayerController::CameraInput);
            EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ADTS_GamePlayPlayerController::JumpKeyStarted);
            EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &ADTS_GamePlayPlayerController::JumpKeyCompleted);
            EnhancedInput->BindAction(OpenInventoryUIAction, ETriggerEvent::Started, this, &ADTS_GamePlayPlayerController::OpenInventoryUI);
            EnhancedInput->BindAction(OpenCreaftingUIAction, ETriggerEvent::Started, this, &ADTS_GamePlayPlayerController::OpenCreaftingUI);
        }
        else if (CurrentMappingContext == InventoryInputMappingContext)
        {
            EnhancedInput->BindAction(CloseInventoryAction, ETriggerEvent::Started, this, &ADTS_GamePlayPlayerController::CloseInventoryUI);
        }
        else if (CurrentMappingContext == CreaftingInputMappingContext) {
            EnhancedInput->BindAction(CloseCreaftingAction, ETriggerEvent::Started, this, &ADTS_GamePlayPlayerController::CloseCreaftingUI);
        }
    }
}

void ADTS_GamePlayPlayerController::MoveInput(const FInputActionValue& value)
{
    if (GetPawn()) {
        // 빙희한 폰이 있고
        ABasePlayerCharacter* CurrentPlayerCharacter = Cast<ABasePlayerCharacter>(GetPawn());
        if (CurrentPlayerCharacter) {
            // ABasePlayerCharacter로 형변환이 가능하면
            FVector2D MovementVector = value.Get<FVector2D>();

            // find out which way is forward
            const FRotator Rotation = GetControlRotation();
            const FRotator YawRotation(0, Rotation.Yaw, 0);
            const FRotator YawRollRotation(0, Rotation.Yaw, Rotation.Roll);

            // get forward vector
            const FVector ForwardDirection = UKismetMathLibrary::GetForwardVector(YawRotation);

            // get right vector 
            const FVector RightDirection = UKismetMathLibrary::GetRightVector(YawRollRotation);

            CurrentPlayerCharacter->AddMovementInput(ForwardDirection, MovementVector.Y);
            CurrentPlayerCharacter->ServerSetMoveX(MovementVector.X);

            CurrentPlayerCharacter->AddMovementInput(RightDirection, MovementVector.X);
            CurrentPlayerCharacter->ServerSetMoveY(MovementVector.Y);

            bAutoMove = false;
        }
        else {
            UE_LOG(LogTemp, Log, TEXT("캐릭터 가져오기 실패"));
        }
    }
    else {
        UE_LOG(LogTemp, Log, TEXT("빙의한 폰 없음"));
    }
}

void ADTS_GamePlayPlayerController::CameraInput(const FInputActionValue& value)
{
    if (GetPawn()) {
        // 빙희한 폰이 있고
        ABasePlayerCharacter* CurrentPlayerCharacter = Cast<ABasePlayerCharacter>(GetPawn());
        if (CurrentPlayerCharacter) {
            // ABasePlayerCharacter로 형변환이 가능하면
            FVector2D CameraInputVector = value.Get<FVector2D>();
            CurrentPlayerCharacter->AddControllerYawInput(CameraInputVector.X);
            CurrentPlayerCharacter->AddControllerPitchInput(CameraInputVector.Y);
        }
        else {
            UE_LOG(LogTemp, Log, TEXT("캐릭터 가져오기 실패"));
        }
    }
    else {
        UE_LOG(LogTemp, Log, TEXT("빙의한 폰 없음"));
    }
}

void ADTS_GamePlayPlayerController::JumpKeyStarted()
{
    if (GetPawn()) {
        // 빙희한 폰이 있고
        ABasePlayerCharacter* CurrentPlayerCharacter = Cast<ABasePlayerCharacter>(GetPawn());
        if (CurrentPlayerCharacter) {
            // ABasePlayerCharacter로 형변환이 가능하면
            CurrentPlayerCharacter->Jump();
        }
        else {
            UE_LOG(LogTemp, Log, TEXT("캐릭터 가져오기 실패"));
        }
    }
    else {
        UE_LOG(LogTemp, Log, TEXT("빙의한 폰 없음"));
    }
}

void ADTS_GamePlayPlayerController::JumpKeyCompleted()
{
    if (GetPawn()) {
        // 빙희한 폰이 있고
        ABasePlayerCharacter* CurrentPlayerCharacter = Cast<ABasePlayerCharacter>(GetPawn());
        if (CurrentPlayerCharacter) {
            // ABasePlayerCharacter로 형변환이 가능하면
            CurrentPlayerCharacter->StopJumping();
        }
        else {
            UE_LOG(LogTemp, Log, TEXT("캐릭터 가져오기 실패"));
        }
    }
    else {
        UE_LOG(LogTemp, Log, TEXT("빙의한 폰 없음"));
    }
}

void ADTS_GamePlayPlayerController::OpenInventoryUI()
{
    if (GetHUD()) {
        ADTS_BaseGamePlayHUD* BaseGamePlayHUD = Cast<ADTS_BaseGamePlayHUD>(GetHUD());
        if (BaseGamePlayHUD) 
        {
            BaseGamePlayHUD->OpenPlayerMain();
            ChangeInputMappingContext(EIMC::InventoryUIIMC);
        }
        else {
            FString Message = FString::Printf(TEXT("HUD를 BaseGamePlayHUD로 변환하기 실패"));
            GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, Message); // 화면에 표시
        }
    }
    else {
        FString Message = FString::Printf(TEXT("HUD 불러오기 실패"));
        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, Message); // 화면에 표시
    }
}

void ADTS_GamePlayPlayerController::ReLoadInventory()
{
    ADTS_BaseGamePlayHUD* CurrentHUD = Cast<ADTS_BaseGamePlayHUD>(GetHUD());
    if (CurrentHUD) {
        CurrentHUD->ReLoadInventory();
    }
}

void ADTS_GamePlayPlayerController::ReLoadQuickSlot()
{
    ADTS_BaseGamePlayHUD* CurrentHUD = Cast<ADTS_BaseGamePlayHUD>(GetHUD());
    if (CurrentHUD) {
        CurrentHUD->ReLoadQuickSlot();
    }

    ServerSlotChange(CurrentQuickSlotNum);
}

void ADTS_GamePlayPlayerController::ServerSwitchItem_Implementation(const FInventory& OriginItem, EInventoryType OriginInventoryType, const FInventory& TargetItem, EInventoryType TargetInventoryType)
{
    InventoryComponent->SwitchItem(OriginItem, OriginInventoryType, TargetItem, TargetInventoryType);
}

void ADTS_GamePlayPlayerController::CloseInventoryUI()
{
    if (GetHUD()) {
        ADTS_BaseGamePlayHUD* BaseGamePlayHUD = Cast<ADTS_BaseGamePlayHUD>(GetHUD());
        if (BaseGamePlayHUD)
        {
            BaseGamePlayHUD->ClosePlayerMain();
            ChangeInputMappingContext(EIMC::DefaultIMC);
        }
        else {
            FString Message = FString::Printf(TEXT("HUD를 BaseGamePlayHUD로 변환하기 실패"));
            GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, Message); // 화면에 표시
        }
    }
    else {
        FString Message = FString::Printf(TEXT("HUD 불러오기 실패"));
        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, Message); // 화면에 표시
    }
}

void ADTS_GamePlayPlayerController::OpenCreaftingUI()
{
    if (GetHUD()) {
        ADTS_BaseGamePlayHUD* BaseGamePlayHUD = Cast<ADTS_BaseGamePlayHUD>(GetHUD());
        if (BaseGamePlayHUD)
        {
            BaseGamePlayHUD->OpenCreaftingUI();
            ChangeInputMappingContext(EIMC::CreaftingUIIMC);
        }
        else {
            FString Message = FString::Printf(TEXT("HUD를 BaseGamePlayHUD로 변환하기 실패"));
            GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, Message); // 화면에 표시
        }
    }
    else {
        FString Message = FString::Printf(TEXT("HUD 불러오기 실패"));
        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, Message); // 화면에 표시
    }
}

void ADTS_GamePlayPlayerController::CloseCreaftingUI()
{
    if (GetHUD()) {
        ADTS_BaseGamePlayHUD* BaseGamePlayHUD = Cast<ADTS_BaseGamePlayHUD>(GetHUD());
        if (BaseGamePlayHUD)
        {
            BaseGamePlayHUD->CloseCreaftingUI();
            ChangeInputMappingContext(EIMC::DefaultIMC);
        }
        else {
            FString Message = FString::Printf(TEXT("HUD를 BaseGamePlayHUD로 변환하기 실패"));
            GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, Message); // 화면에 표시
        }
    }
    else {
        FString Message = FString::Printf(TEXT("HUD 불러오기 실패"));
        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, Message); // 화면에 표시
    }
}

void ADTS_GamePlayPlayerController::ClientSetBoss_Implementation(AEnemyBossBase* Boss)
{

    if (!IsValid(Boss)) {
        return;
    }

    if (GetHUD()) {
        ADTS_BaseGamePlayHUD* BaseGamePlayHUD = Cast<ADTS_BaseGamePlayHUD>(GetHUD());
        if (BaseGamePlayHUD)
        {
            BaseGamePlayHUD->BindBossTakeDamage(Boss);
            BaseGamePlayHUD->SetBossHpBarVisibility(true);
            BaseGamePlayHUD->SetBossName(FText::FromString(TEXT("보스")));
            BaseGamePlayHUD->SetBossHpBarPercent(Boss->GetCurrentHealth(), Boss->GetMaxHealth());
        }
        else {
            UKismetSystemLibrary::PrintString(this, TEXT("HUD를 BaseGamePlayHUD로 변환하기 실패"));
        }
    }
    else {
        UKismetSystemLibrary::PrintString(this, TEXT("HUD 불러오기 실패"));
    }
}

void ADTS_GamePlayPlayerController::ClientUnSetBoss_Implementation(AEnemyBossBase* Boss)
{
    if (!IsValid(Boss)) {
        return;
    }

    if (GetHUD()) {
        ADTS_BaseGamePlayHUD* BaseGamePlayHUD = Cast<ADTS_BaseGamePlayHUD>(GetHUD());
        if (BaseGamePlayHUD)
        {
            BaseGamePlayHUD->UnBindBossTakeDamage(Boss);
            BaseGamePlayHUD->SetBossHpBarVisibility(false);
        }
        else {
            UKismetSystemLibrary::PrintString(this, TEXT("HUD를 BaseGamePlayHUD로 변환하기 실패"));
        }
    }
    else {
        UKismetSystemLibrary::PrintString(this, TEXT("HUD 불러오기 실패"));
    }
}

/// <summary>
/// 공격 함수
/// </summary>
void ADTS_GamePlayPlayerController::Attack()
{
    if (GetPawn()) {
        // 빙희한 폰이 있고
        ABasePlayerCharacter* CurrentPlayerCharacter = Cast<ABasePlayerCharacter>(GetPawn());
        if (CurrentPlayerCharacter) {
            // ABasePlayerCharacter로 형변환이 가능하면
            CurrentPlayerCharacter->ServerAttack();
        }
        else {
            UE_LOG(LogTemp, Log, TEXT("캐릭터 가져오기 실패"));
        }
    }
    else {
        UE_LOG(LogTemp, Log, TEXT("빙의한 폰 없음"));
    }
}

/// <summary>
/// 강공격 함수
/// </summary>
void ADTS_GamePlayPlayerController::StrongAttack()
{
    if (GetPawn()) {
        // 빙희한 폰이 있고
        ABasePlayerCharacter* CurrentPlayerCharacter = Cast<ABasePlayerCharacter>(GetPawn());
        if (CurrentPlayerCharacter) {
            // ABasePlayerCharacter로 형변환이 가능하면
            CurrentPlayerCharacter->ServerStrongAttack();
        }
        else {
            UE_LOG(LogTemp, Log, TEXT("캐릭터 가져오기 실패"));
        }
    }
    else {
        UE_LOG(LogTemp, Log, TEXT("빙의한 폰 없음"));
    }
}

void ADTS_GamePlayPlayerController::BowAiming()
{
    if (GetPawn()) {
        // 빙희한 폰이 있고
        ABasePlayerCharacter* CurrentPlayerCharacter = Cast<ABasePlayerCharacter>(GetPawn());
        if (CurrentPlayerCharacter) {
            // ABasePlayerCharacter로 형변환이 가능하면
            CurrentPlayerCharacter->ServerBowAiming();
            CurrentPlayerCharacter->SetbOrientRotationToMovement(false);
        }
        else {
            UE_LOG(LogTemp, Log, TEXT("캐릭터 가져오기 실패"));
        }
    }
    else {
        UE_LOG(LogTemp, Log, TEXT("빙의한 폰 없음"));
    }
}

void ADTS_GamePlayPlayerController::BowShot()
{
    if (GetPawn()) {
        // 빙희한 폰이 있고
        ABasePlayerCharacter* CurrentPlayerCharacter = Cast<ABasePlayerCharacter>(GetPawn());
        if (CurrentPlayerCharacter) {
            // ABasePlayerCharacter로 형변환이 가능하면
            // 공격
            CurrentPlayerCharacter->ServerBowShot();
            CurrentPlayerCharacter->SetbOrientRotationToMovement(true);
        }
        else {
            UE_LOG(LogTemp, Log, TEXT("캐릭터 가져오기 실패"));
        }
    }
    else {
        UE_LOG(LogTemp, Log, TEXT("빙의한 폰 없음"));
    }
}

void ADTS_GamePlayPlayerController::BowAimReleased()
{
    if (GetPawn()) {
        // 빙희한 폰이 있고
        ABasePlayerCharacter* CurrentPlayerCharacter = Cast<ABasePlayerCharacter>(GetPawn());
        if (CurrentPlayerCharacter) {
            // ABasePlayerCharacter로 형변환이 가능하면
            CurrentPlayerCharacter->ServerBowAimReleased();
            CurrentPlayerCharacter->SetbOrientRotationToMovement(true);
        }
        else {
            UE_LOG(LogTemp, Log, TEXT("캐릭터 가져오기 실패"));
        }
    }
    else {
        UE_LOG(LogTemp, Log, TEXT("빙의한 폰 없음"));
    }
}

/// <summary>
/// 퀵슬롯 1 ~ 5로 하는 함수
/// </summary>
void ADTS_GamePlayPlayerController::QuickSlot1()
{
    if (GetPawn()) {
        // 빙희한 폰이 있고
        ABasePlayerCharacter* CurrentPlayerCharacter = Cast<ABasePlayerCharacter>(GetPawn());
        if (CurrentPlayerCharacter) {
            // ABasePlayerCharacter로 형변환이 가능하면
            ChangeInputMappingContext(EIMC::DefaultIMC);
            CurrentPlayerCharacter->ResetbIsAiming();
        }
        else {
            UE_LOG(LogTemp, Log, TEXT("캐릭터 가져오기 실패"));
        }
    }
    else {
        UE_LOG(LogTemp, Log, TEXT("빙의한 폰 없음"));
    }

    SelectQuickSlot(0);

   
}

void ADTS_GamePlayPlayerController::QuickSlot2()
{
    if (GetPawn()) {
        // 빙희한 폰이 있고
        ABasePlayerCharacter* CurrentPlayerCharacter = Cast<ABasePlayerCharacter>(GetPawn());
        if (CurrentPlayerCharacter) {
            // ABasePlayerCharacter로 형변환이 가능하면
            ChangeInputMappingContext(EIMC::DefaultIMC);
            CurrentPlayerCharacter->ResetbIsAiming();
        }
        else {
            UE_LOG(LogTemp, Log, TEXT("캐릭터 가져오기 실패"));
        }
    }
    else {
        UE_LOG(LogTemp, Log, TEXT("빙의한 폰 없음"));
    }

    SelectQuickSlot(1);
}

void ADTS_GamePlayPlayerController::QuickSlot3()
{
    if (GetPawn()) {
        // 빙희한 폰이 있고
        ABasePlayerCharacter* CurrentPlayerCharacter = Cast<ABasePlayerCharacter>(GetPawn());
        if (CurrentPlayerCharacter) {
            // ABasePlayerCharacter로 형변환이 가능하면
            ChangeInputMappingContext(EIMC::DefaultIMC);
            CurrentPlayerCharacter->ResetbIsAiming();
        }
        else {
            UE_LOG(LogTemp, Log, TEXT("캐릭터 가져오기 실패"));
        }
    }
    else {
        UE_LOG(LogTemp, Log, TEXT("빙의한 폰 없음"));
    }

    SelectQuickSlot(2);
}

void ADTS_GamePlayPlayerController::QuickSlot4()
{
    if (GetPawn()) {
        // 빙희한 폰이 있고
        ABasePlayerCharacter* CurrentPlayerCharacter = Cast<ABasePlayerCharacter>(GetPawn());
        if (CurrentPlayerCharacter) {
            // ABasePlayerCharacter로 형변환이 가능하면
            ChangeInputMappingContext(EIMC::BowIMC);
        }
        else {
            UE_LOG(LogTemp, Log, TEXT("캐릭터 가져오기 실패"));
        }
    }
    else {
        UE_LOG(LogTemp, Log, TEXT("빙의한 폰 없음"));
    }

    SelectQuickSlot(3);
}

void ADTS_GamePlayPlayerController::QuickSlot5()
{

    SelectQuickSlot(4);
}

void ADTS_GamePlayPlayerController::SelectQuickSlot(int SlotNum)
{
    CurrentQuickSlotNum = SlotNum;

    FString Message;
    if (!HasAuthority()) {
        if (GetHUD()) {
            ADTS_BaseGamePlayHUD* BaseGamePlayHUD = Cast<ADTS_BaseGamePlayHUD>(GetHUD());
            if (BaseGamePlayHUD)
            {
                BaseGamePlayHUD->SelectQuickSlot(SlotNum);
            }
            else {
                Message = FString::Printf(TEXT("BaseGamePlayHUD Cast Fail"));
                GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, Message); // 화면에 표시
            }
        }
        else {
            Message = FString::Printf(TEXT("Call Hud Fail"));
            GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, Message); // 화면에 표시
        }

    }
    else {
        
    }

    ServerSlotChange(SlotNum);

}

/// <summary>
/// 구조물을 설치하는 함수
/// </summary>
void ADTS_GamePlayPlayerController::CreateStructure()
{
    // 구조물을 설치하는 로직 필요
    
    /* 주연 수정 */
    FString Message = FString::Printf(TEXT("구조물 설치"));
    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, Message); // 화면에 표시

    APawn* PossessedPawn = GetPawn();
    if (!PossessedPawn)
    {
        return;
    }

    ABasePlayerCharacter* MyCharacter = Cast<ABasePlayerCharacter>(PossessedPawn);
    if (!MyCharacter)
    {
        return;
    }

    UCraftManager* CraftManager = MyCharacter->FindComponentByClass<UCraftManager>();
    if (!CraftManager)
    {
        return;
    }

    //ChangeInputMappingContext(EIMC::DefaultIMC);

    CraftManager->TryPlaceBuilding();
  }

/// <summary>
/// 구조물 설치를 취소하는 함수
/// </summary>
void ADTS_GamePlayPlayerController::CancleCraftingMode()
{
    ChangeInputMappingContext(EIMC::DefaultIMC);
    // 제작 버튼을 눌렀을 시 빠져나간 아이템을 다시 넣어주는 로직 필요
    // 표시 되고 있는 제작을 캔슬하는 로직 필요

        /* 주연 수정 */

    APawn* PossessedPawn = GetPawn();
    if (!PossessedPawn)
    {
        return;
    }

    ABasePlayerCharacter* MyCharacter = Cast<ABasePlayerCharacter>(PossessedPawn);
    if (!MyCharacter)
    {
        return;
    }

    UCraftManager* CraftManager = MyCharacter->FindComponentByClass<UCraftManager>();
    if (!CraftManager)
    {
        return;
    }

    CraftManager->EndCraftMode();

    FString Message = FString::Printf(TEXT("크래프팅 모드 취소"));
    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, Message); // 화면에 표시
}

/// <summary>
/// 구조물을 반시계 방향으로 돌리는 함수
/// </summary>
void ADTS_GamePlayPlayerController::StructureTurnLeft()
{
    FString Message = FString::Printf(TEXT("구조물 왼쪽으로 회전"));
    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, Message); // 화면에 표시
    // 회전하는 왼쪽으로 회전하는 로직 필요

    /* 주연 수정 */

    APawn* PossessedPawn = GetPawn();
    if (!PossessedPawn)
    {
        return;
    }

    ABasePlayerCharacter* MyCharacter = Cast<ABasePlayerCharacter>(PossessedPawn);
    if (!MyCharacter)
    {
        return;
    }

    UCraftManager* CraftManager = MyCharacter->FindComponentByClass<UCraftManager>();
    if (!CraftManager)
    {
        return;
    }

    CraftManager->RotateLeftGhostMesh();
}

/// <summary>
/// 구조물을 시계 방향으로 돌리는 함수
/// </summary>
void ADTS_GamePlayPlayerController::StructureTurnRight()
{
    FString Message = FString::Printf(TEXT("구조물 오른쪽으로 회전"));
    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, Message); 
    // 회전하는 왼쪽으로 회전하는 로직 필요

    /* 주연 수정 */

    APawn* PossessedPawn = GetPawn();
    if (!PossessedPawn)
    {
        return;
    }

    ABasePlayerCharacter* MyCharacter = Cast<ABasePlayerCharacter>(PossessedPawn);
    if (!MyCharacter)
    {
        return;
    }

    UCraftManager* CraftManager = MyCharacter->FindComponentByClass<UCraftManager>();
    if (!CraftManager)
    {
        return;
    }

    CraftManager->RotateRightGhostMesh();
}

/// <summary>
/// 상호작용을 하는 함수
/// </summary>
void ADTS_GamePlayPlayerController::Interaction()
{

    ServerNavMove();
    
    if (InteractionActor) {
        IDTS_BPIInteraction* InterfaceInstance = Cast<IDTS_BPIInteraction>(InteractionActor);
        if (InterfaceInstance)
        {
            switch (InterfaceInstance->Execute_GetInteractionType(InteractionActor))
            {
            case EInteraction::Item:
                bAutoMove = true;
                bIsInteractionReady = true;
                break;
            case EInteraction::Bush:
                bAutoMove = true;
                bIsInteractionReady = true;
                break;
            case EInteraction::Tree:
                bAutoMove = true;
                bIsInteractionReady = true;
                break;
            case EInteraction::Rock:
                bAutoMove = true;
                bIsInteractionReady = true;
                break;
            case EInteraction::Fire:
                if (GetCurrentPlayerCharacter()) {
                    InterfaceInstance->Execute_Interaction(InteractionActor, this, GetCurrentPlayerCharacter());
                }
                break;
            default:
                break;
            }
        }
        UKismetSystemLibrary::PrintString(this, TEXT("상호작용 동작 하는중 시작"));

    }
    
}

void ADTS_GamePlayPlayerController::ServerSlotChange_Implementation(int SlotNum)
{
    ABasePlayerCharacter* CurrentPlayerCharacter = Cast<ABasePlayerCharacter>(GetPawn());
    if (!CurrentPlayerCharacter) {
        UKismetSystemLibrary::PrintString(this, TEXT("BaseCharacter로 캐스트 실패"));
        return;
    }

    if (InventoryComponent->QuickSlot[SlotNum].InventoryCoordinate.FillSlot) {
        // 퀵슬롯에 아이템이 할당되어 있다면 데이터 테이블을 가져와서 아이템을 소환한 뒤 넣어줌
        UDataTable* ItemDataTable;
        ItemDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Leesh/Table/DT_TestItemInfo.DT_TestItemInfo"));	// 데이터 테이블 불러옴
        if (ItemDataTable) {
            // 데이터 테이블이 존재하면
            FName ItemRowName = FName(*InventoryComponent->QuickSlot[SlotNum].ItemName);
            FTestItemInfo* DTItem = ItemDataTable->FindRow<FTestItemInfo>(ItemRowName, TEXT("General"));
            if (DTItem) {
                CurrentPlayerCharacter->AttachItem(GetWorld()->SpawnActor<ASHBaseItem>(DTItem->Item));
            }
            else {
                UKismetSystemLibrary::PrintString(this, TEXT("RowName에 해당하는 아이템이 없음"));
            }

        }
        else {
            UKismetSystemLibrary::PrintString(this, TEXT("데이터 테이블 불러오기 실패"));
        }
    }
    else {
        CurrentPlayerCharacter->DetachItem();
    }
}

void ADTS_GamePlayPlayerController::ServerSpawnItemByItemName_Implementation(const FString& ItemName, bool bThrowItem, FVector Impulse, bool bThrowPlayerFront)
{
    UDataTable* ItemDataTable;
    ItemDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Leesh/Table/DT_TestItemInfo.DT_TestItemInfo"));	// 데이터 테이블 불러옴
    if (ItemDataTable) {
        // 데이터 테이블이 존재하면
        FName ItemRowName = FName(*ItemName);
        FTestItemInfo* DTItem = ItemDataTable->FindRow<FTestItemInfo>(ItemRowName, TEXT("General"));
        if (DTItem) {
            if (DTItem->Item) {
                

                ASHBaseItem* SpawnItem = GetWorld()->SpawnActor<ASHBaseItem>(DTItem->Item, GetPawn()->GetActorLocation() + GetPawn()->GetActorForwardVector() * 100 , FRotator::ZeroRotator);
                if (bThrowItem) {
                    if (bThrowPlayerFront) {
                        SpawnItem->Mesh->AddImpulse(GetPawn()->GetActorForwardVector() * 10000);
                    }
                    else {
                        SpawnItem->Mesh->AddImpulse(Impulse);
                    }
                }
                    
            }
            else {
                UKismetSystemLibrary::PrintString(this, TEXT("ItemName에 해당하는 아이템이 없음"));
            }
            
        }
        else {
            UKismetSystemLibrary::PrintString(this, TEXT("RowName에 해당하는 아이템이 없음"));
        }

    }
    else {
        UKismetSystemLibrary::PrintString(this, TEXT("데이터 테이블 불러오기 실패"));
    }
}

void ADTS_GamePlayPlayerController::SetInteractionActor_Implementation(AActor* Actor)
{
    InteractionActor = Actor;
}

ABasePlayerCharacter* ADTS_GamePlayPlayerController::GetCurrentPlayerCharacter()
{
    if (GetPawn()) {
        ABasePlayerCharacter* CurrentPlayerCharacter = Cast<ABasePlayerCharacter>(GetPawn());
        if (CurrentPlayerCharacter) {
            return CurrentPlayerCharacter;
        }
        else {
            return nullptr;
        }
    }
    else {
        return nullptr;
    }
}

FCharacterStat ADTS_GamePlayPlayerController::GetPlayerStat()
{
    if (GetPlayerState<ADTS_BasePlayerState>()) {
        return GetPlayerState<ADTS_BasePlayerState>()->GetAllPlayerStat();
    }
    return FCharacterStat(0,0,0,0,0,0,0,0,0,0,0);
}

void ADTS_GamePlayPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ADTS_GamePlayPlayerController, NewDir);
    DOREPLIFETIME(ADTS_GamePlayPlayerController, InteractionActor);

}


void ADTS_GamePlayPlayerController::ServerNavMove_Implementation()
{
    if (InteractionActor) {
        // 상호작용할 액터가 있고
        ABasePlayerCharacter* CurrentPlayerCharacter = Cast<ABasePlayerCharacter>(GetPawn());
        if (CurrentPlayerCharacter) {
            // 플레이어 캐릭터가 있으면
            UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(
                GetWorld(),
                CurrentPlayerCharacter->GetActorLocation(),
                InteractionActor->GetActorLocation()
            );

            if (NavPath && NavPath->PathPoints.Num() > 1)
            {
                // 다음 경로 방향을 구해서 AddMovementInput으로 이동
                FVector NextPoint = NavPath->PathPoints[1];
                NewDir = (NextPoint - CurrentPlayerCharacter->GetActorLocation()).GetSafeNormal();
                
            }
            else {
                UKismetSystemLibrary::PrintString(this, TEXT("경로가 없거나 다음 장소가 없음"));
                bAutoMove = false;
            }
        }
        else {

        }

    }
    else {

    }
}



/// <summary>
/// 상호 작용이 가능한 물체를 트레이스로 차즌ㄴ 함수
/// </summary>
void ADTS_GamePlayPlayerController::FindInteractionActor_Implementation()
{
    FVector StartTrace;
    FVector EndTrace;
    FRotator PlayerViewRot;

    GetPlayerViewPoint(StartTrace, PlayerViewRot);
    FVector AddVector = UKismetMathLibrary::GetForwardVector(PlayerViewRot);
    
    StartTrace = StartTrace + (AddVector * 250);

    EndTrace = StartTrace + (AddVector * 250);

    TArray<AActor*> IgnoreActorArr;

    if (GetCurrentPlayerCharacter()) {
        if (GetCurrentPlayerCharacter()->CurrentEquipItem) {
            IgnoreActorArr.Add(GetCurrentPlayerCharacter()->CurrentEquipItem);
        }
    }

    TArray<TEnumAsByte<EObjectTypeQuery>> TraceObject;
    // "Player"라는 오브젝트 채널을 찾아서 변환 후 추가
    EObjectTypeQuery PlayerObjectType = UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1); // 적절한 채널 번호 사용
    EObjectTypeQuery InteractionObjectType = UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel4); // 적절한 채널 번호 사용
    TraceObject.Add(PlayerObjectType);
    TraceObject.Add(InteractionObjectType);

    TArray <FHitResult> HitResultArray;
    FHitResult HitResult;

    if (UKismetSystemLibrary::CapsuleTraceSingleForObjects(GetWorld(),
        StartTrace,
        EndTrace,
        10.f,
        20.f,
        TraceObject,
        false,
        IgnoreActorArr,
        EDrawDebugTrace::None,   // 트레이스 디버그 
        HitResult,
        true,
        FLinearColor::Red,
        FLinearColor::Green,
        0.f)
        )
    {
        if (!bAutoMove) {
            InteractionActorActionSingle(HitResult);
        }
    }
    else {
        if (!bAutoMove && !bIsInteractionReady) {
            InteractionActor = nullptr;
        }
    }

}

/// <summary>
/// 멀티 켑슐 용 상호작용 창 띄우기
/// </summary>
/// <param name="ResultArr"></param>
void ADTS_GamePlayPlayerController::InteractionActorAction_Implementation(const TArray<FHitResult>& ResultArr)
{

}

void ADTS_GamePlayPlayerController::InteractionActorActionSingle_Implementation(const FHitResult& HitResult)
{
    if (HitResult.GetActor() && HitResult.GetActor()->Implements<UDTS_BPIInteraction>()) {
        IDTS_BPIInteraction* InterfaceInstance = Cast<IDTS_BPIInteraction>(HitResult.GetActor());
        if (InterfaceInstance)
        {
            UDTS_BaseInteractionUserWidget* CurrentInteractionWidget = InterfaceInstance->Execute_GetBaseInteractionWidget(HitResult.GetActor());
            if (CurrentInteractionWidget) {
                ShowWidgetClient(CurrentInteractionWidget);
            }

            if (!bAutoMove) {
                //FString Name;
                //HitResult.GetActor()->GetName(Name);
                //UKismetSystemLibrary::PrintString(this, TEXT("InteractionActor 재정의") + Name);
                SetInteractionActor(HitResult.GetActor());
            }
        }
    }
}

void ADTS_GamePlayPlayerController::ShowWidgetClient_Implementation(UDTS_BaseInteractionUserWidget* InteractionUserWidget) {
    if (InteractionUserWidget) {
        InteractionUserWidget->SetVisibilityWidget();
    }
}
