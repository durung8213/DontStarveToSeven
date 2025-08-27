// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kimmh/Character/Controller/BasePlayerController.h"

#include "DTS_GamePlayPlayerController.generated.h"

enum class EIMC
{
	DefaultIMC,
	InventoryUIIMC,
	CreaftingUIIMC,
	CreaftingIMC,
	BowIMC
};

/**
 *
 */
UCLASS()
class DONTSTARVETOSEVEN_API ADTS_GamePlayPlayerController : public ABasePlayerController
{
	GENERATED_BODY()

public:
	ADTS_GamePlayPlayerController();

	virtual void OnNetCleanup(UNetConnection* Connection) override;

	void ChangeInputMappingContext(EIMC IMC);

	UFUNCTION(Client, BlueprintCallable, Reliable)
	void FindNormalEnemy();

	/// <summary>
	/// 상호작용이 가능한 물체를 찾는 함수
	/// </summary>
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void FindInteractionActor();
	virtual void FindInteractionActor_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void InteractionActorAction(const TArray<FHitResult>& ResultArr);
	void InteractionActorAction_Implementation(const TArray<FHitResult>& ResultArr);


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void InteractionActorActionSingle(const FHitResult& HitResult);
	void InteractionActorActionSingle_Implementation(const FHitResult& HitResult);

	/// <summary>
	/// 인벤토리를 다시 로드하는 함수
	/// </summary>
	void ReLoadInventory();

	/// <summary>
	/// 인벤토리를 다시 로드하는 함수
	/// </summary>
	void ReLoadQuickSlot();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerSwitchItem(const FInventory& OriginItem, EInventoryType OriginInventoryType, const FInventory& TargetItem, EInventoryType TargetInventoryType);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void AddItemToInventoryByName(const FString& ItemName, int Count = 1);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void RemoveItemToInventoryByName(const FString& ItemName, int Count = 1);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerRemoveItem(const FInventory& Item, EInventoryType InventoryType);

	UFUNCTION(NetMulticast, Reliable)
	void ClientSetBoss(class AEnemyBossBase* Boss);

	UFUNCTION(NetMulticast, Reliable)
	void ClientUnSetBoss(class AEnemyBossBase* Boss);

	int SearchItemCountToInventory(FString InItemName);

	int SearchItemCountToQuickSlot(FString InItemName);

	UFUNCTION(Server, Reliable)
	void ServerSpawnItemByItemName(const FString& ItemName, bool bThrowItem = false, FVector Impulse = FVector(0,0,0), bool bThrowPlayerFront = true);

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupInputComponent() override;


	void RebindActions();

	// 캐릭터 상하좌우
	void MoveInput(const FInputActionValue& value);

	// 캐릭터 카메라 회전
	void CameraInput(const FInputActionValue& value);

	/// <summary>
	/// 점프버튼이 눌렸을 때 점프하는 함수
	/// </summary>
	void JumpKeyStarted();

	/// <summary>
	/// 점프 버튼이 릴리즈 되었을 때 점프를 해제하는 함수
	/// </summary>
	void JumpKeyCompleted();

	/// <summary>
	/// 인벤토리를 여는 버튼 (B)를 눌렀을 때 인벤토리를 여는 함수
	/// </summary>
	void OpenInventoryUI();

	/// <summary>
	/// 인벤토리를 닫는 함수
	/// </summary>
	void CloseInventoryUI();

	/// <summary>
	/// 크래프팅 UI를 여는 함수
	/// </summary>
	void OpenCreaftingUI();

	/// <summary>
	/// 크래프팅 UI를 닫는 함수
	/// </summary>
	void CloseCreaftingUI();




	/// <summary>
	/// 일반 공격 함수
	/// </summary>
	UFUNCTION()
	void Attack();

	/// <summary>
	/// 강공격 함수
	/// </summary>
	UFUNCTION()
	void StrongAttack();

	/// <summary>
	/// 활 조준 중
	/// </summary>
	UFUNCTION()
	void BowAiming();

	/// <summary>
	/// 활 쏘기
	/// </summary>
	UFUNCTION()
	void BowShot();

	/// <summary>
	/// 활 당기는거 취소
	/// </summary>
	UFUNCTION()
	void BowAimReleased();

	/// <summary>
	/// 퀵슬롯 1
	/// </summary>
	void QuickSlot1();

	/// <summary>
	/// 퀵슬롯 2
	/// </summary>
	void QuickSlot2();

	/// <summary>
	/// 퀵슬롯 3
	/// </summary>
	void QuickSlot3();

	/// <summary>
	/// 퀵슬롯 4
	/// </summary>
	void QuickSlot4();

	/// <summary>
	/// 퀵슬롯 5
	/// </summary>
	void QuickSlot5();

	/// <summary>
	/// 퀵슬롯 선택을 했을 때 불리는 함수
	/// </summary>
	/// <param name="SlotNum"></param>
	void SelectQuickSlot(int SlotNum);

	/// -------------------------- 건축 모드 관련 ------------------------

	/// <summary>
	/// 구조물을 제작하는 함수
	/// </summary>
	virtual void CreateStructure();

	/// <summary>
	/// 구조물 제작 모드를 캔슬하는 함수
	/// </summary>
	virtual void CancleCraftingMode();

	/// <summary>
	/// 구조물을 왼쪽으로 돌리는 함수
	/// </summary>
	virtual void StructureTurnLeft();

	/// <summary>
	/// 구조물을 오른쪽을 돌리는 키에 바인딩 될 함수
	/// </summary>
	virtual void StructureTurnRight();

	/// -------------------------- 건축 모드 관련 끝 ------------------------

	/// -------------------------- 상호작용 관련 ------------------------

	/// <summary>
	/// 받아온 위젯을 클라이턴트에서 보이게 하는 것
	/// </summary>
	UFUNCTION(Client, BlueprintCallable, Reliable)
	void ShowWidgetClient(UDTS_BaseInteractionUserWidget* InteractionUserWidget);

	/// <summary>
	/// 상호작용을 하는 함수
	/// </summary>
	UFUNCTION()
	void Interaction();

	/// <summary>
	/// nav 움직임을 함
	/// </summary>
	UFUNCTION(Server, Reliable)
	void ServerNavMove();

	void InteractionFalse();

	UFUNCTION(Server, Reliable)
	void SetInteractionActor(AActor* Actor);

	/// -------------------------- 상호작용 관련 끝 ------------------------

	/// -------------------------- 퀵슬롯 아이템 장착 관련 ------------------
	UFUNCTION(Server, Reliable)
	void ServerSlotChange(int SlotNum);


	UFUNCTION()
	ABasePlayerCharacter* GetCurrentPlayerCharacter();

public:
	/// ------------------------- 저장 관련 함수 --------------------------
	struct FCharacterStat GetPlayerStat();


	// 서버(게임모드) → 클라이언트: 닉네임 요청
	UFUNCTION(Client, Reliable, BlueprintCallable)
	void ClientRequestNickname();

	UFUNCTION(BlueprintCallable)
	void GetNickName();

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "DataManage")
	void ServerSetNickName(const FString& NewNickName);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "DataManage")
	void ServerTossInfoToGameMode();

	/// <summary>
	///	변수를 리플리케이트 하기 위해 필요한 함수(참고하라고)
	/// </summary>
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class USHInventoryComponent* InventoryComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class USHInventoryComponent* QuickSlotInventoryComponent;

	/// <summary>
	/// 현재 맵핑 컨텍스트
	/// </summary>
	UPROPERTY()
	class UInputMappingContext* CurrentMappingContext;

	/// <summary>
	/// 기본 상태 맵핑 컨텍스트
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultInputMappingContext;

	/// <summary>
	/// WASD 이동 액션
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/// <summary>
	/// 카메라 (시야 이동) 액션
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* CameraAction;

	/// <summary>
	/// 점프 스타트
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/// <summary>
	/// 일반 공격 액션
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AttackAction;

	/// <summary>
	/// 강공격 액션
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* StrongAttackAction;

	/// <summary>
	/// 퀵슬롯1
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* QuickSlot1Action;

	/// <summary>
	/// 퀵슬롯 2
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* QuickSlot2Action;

	/// <summary>
	/// 퀵슬로 3
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* QuickSlot3Action;

	/// <summary>
	/// 퀵 슬롯 4
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* QuickSlot4Action;

	/// <summary>
	/// 퀵 슬롯 5
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* QuickSlot5Action;

	/// <summary>
	/// 인벤토리 UI를 여는 액션
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* OpenInventoryUIAction;

	/// <summary>
	/// 크래프팅 UI를 여는 액션
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* OpenCreaftingUIAction;

	/// <summary>
	/// 상호작용 하는 액션
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InteractionAction;


	/// <summary>
	/// 활 전용 IMC
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* BowInputMappingContext;

	/// <summary>
	/// 조준
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AimingAction;

	/// <summary>
	/// 활 사격
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ShotAction;

	/// <summary>
	/// 활 조준 취소
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AimingReleasedAction;

	/// <summary>
	/// 인벤토리 UI를 열었을 때 사용할 맵핑 컨텍스트
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* InventoryInputMappingContext;

	/// <summary>
	/// 인벤토리 UI를 닫는 액션
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* CloseInventoryAction;

	/// <summary>
	/// 크래프팅 UI를 열었을 때 사용할 맵핑 컨택스트
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* CreaftingInputMappingContext;

	/// <summary>
	/// 크래프팅 UI를 닫는 액션
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* CloseCreaftingAction;

	/// <summary>
	/// 크래프팅 모드일 떄 사용할
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* CreaftingModeInputMappingContext;

	/// <summary>
	/// 제작하는 액션
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* CreateAction;

	/// <summary>
	/// 제작을 캔슬하는 액션
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* CancleCreaftingModeAction;

	/// <summary>
	/// 제작 구조물을 왼쪽으로 회전하는 액션
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* RotateLeft;

	/// <summary>
	/// 제작 구조물을 오른쪽으로 회전하는 액션
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* RotateRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	AActor* InteractionActor;



	UPROPERTY(Replicated)
	FVector NewDir;

	UPROPERTY(Replicated)
	bool bAutoMove = false;

	bool bIsInteractionReady = false;

	int CurrentQuickSlotNum = 0;


	/// <summary>
	/// 이상화 제작, 게임 저장 및 닉네임 관련
	/// </summary>
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "Inventory")
	FString NickName = TEXT("");
};
