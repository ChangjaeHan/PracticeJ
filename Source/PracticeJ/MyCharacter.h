// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "Animation/AnimInstance.h"
#include "MyAnimInstance.h"
#include "GameFramework/Character.h"

#include "InputMappingContext.h"

#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Components/SphereComponent.h"
#include "Animation/SkeletalMeshActor.h"
#include "MyWeapon.h"
#include "MyAttackComponent.h"
#include "MyHealthComponent.h"

#include "InteractionInterface.h"
#include "MyCharacter.generated.h"

class AMyHUD;

USTRUCT()
struct FInteractionData
{
	GENERATED_USTRUCT_BODY()


	FInteractionData() : CurrentInteractable(nullptr), LastInteractionCheckTime(0.0f)
	{

	};

	//UPROPERTY()
	AActor* CurrentInteractable;
	
	//UPROPERTY()
	float LastInteractionCheckTime;

};



UCLASS()
class PRACTICEJ_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	//매핑컨텍스트를 헤더 파일에 프로퍼티로 노출합니다..
	UPROPERTY(EditAnywhere, Category = "Input")
	TSoftObjectPtr<UInputMappingContext> InputMapping;

public:


	//달리기 성질
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	float WalkSpeed = 300.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	float RunSpeed = 900.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	float SprintMultiplier;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	float JumpPower = 800.0f;
	

	//스태미나 성질
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	bool bIsRunning;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	float MaxStamina = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	float CurStamina = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	float StaminaDrainTime = 0.03f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	float StaminaRefillTime = 0.05f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	float DelayBeforeRefill = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	float CurRefillDelayTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	bool bHasStamina;


	//무기 픽업 여부
	bool bHasWeapon;
	AMyWeapon* CurrentWeapon;


	float PunchDamage = 5.0f;

	UPROPERTY(BlueprintReadWrite)
	bool bIsAttacking;

	UPROPERTY(BlueprintReadWrite)
	bool bHasDamaged;
	


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UMyHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UMyAttackComponent* AttackComponent;

protected:

	//카메라
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* CameraArm;

	/*
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* FirstPersonCamera;
	*/

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* ThirdPersonCamera;

	
	FORCEINLINE bool IsInteracting() const { return GetWorldTimerManager().IsTimerActive(TimerHandleInteraction); };

	AMyHUD* HUD;


	//Interface

	UPROPERTY(VisibleAnywhere, Category = "Character | Interaction")
	TScriptInterface<IInteractionInterface> TargetInteractable;

	float InteractionCheckFrequency;
	float InteractionCheckDistance;

	FTimerHandle TimerHandleInteraction;

	FInteractionData InteractionData;

	//doing action (인터페이스의 함수와는 다른것. interaction을 하는 주체이냐 아니냐)
	void PerformInteractionCheck();
	void FoundInteractable(AActor* NewInteractable);
	void NoInteractableFound();
	void BeginInteract();
	void EndInteract();
	void Interact();





private:

	/**이벤트 키(디버깅*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* EventAction;

	
	/** 이동*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** 바라보기*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** 점프*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** 달리기*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SprintAction;


	/** 무기 픽업*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* PickupAction;

	/** 드랍*/

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* DropAction;


	/** 펀치 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AttackAction;

	/** 상호작용*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InteractAction;


protected:

	//이벤트 실행(디버깅)
	void DoEvent();

	//이동
	void Move(const FInputActionValue& Value);

	//바라보기
	void Look(const FInputActionValue& Value);

	//달리기
	void StartSprint();
	void EndSprint();

	//스태미나 업데이트
	void UpdateStamina();


	void DropWeapon();

	//공격 호출 함수 1. normal Attack 2. sword attack
	void DoAttack();

	void PerformNormalAttack();

	void PerformSwordAttack();

	UFUNCTION(BlueprintCallable)
	void LineTrace();




};
