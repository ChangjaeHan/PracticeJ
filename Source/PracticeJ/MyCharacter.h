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


	//�������ؽ�Ʈ�� ��� ���Ͽ� ������Ƽ�� �����մϴ�..
	UPROPERTY(EditAnywhere, Category = "Input")
	TSoftObjectPtr<UInputMappingContext> InputMapping;

public:


	//�޸��� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	float WalkSpeed = 300.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	float RunSpeed = 900.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	float SprintMultiplier;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	float JumpPower = 800.0f;
	

	//���¹̳� ����
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


	//���� �Ⱦ� ����
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

	//ī�޶�
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

	//doing action (�������̽��� �Լ��ʹ� �ٸ���. interaction�� �ϴ� ��ü�̳� �ƴϳ�)
	void PerformInteractionCheck();
	void FoundInteractable(AActor* NewInteractable);
	void NoInteractableFound();
	void BeginInteract();
	void EndInteract();
	void Interact();





private:

	/**�̺�Ʈ Ű(�����*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* EventAction;

	
	/** �̵�*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** �ٶ󺸱�*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** ����*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** �޸���*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SprintAction;


	/** ���� �Ⱦ�*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* PickupAction;

	/** ���*/

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* DropAction;


	/** ��ġ */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AttackAction;

	/** ��ȣ�ۿ�*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InteractAction;


protected:

	//�̺�Ʈ ����(�����)
	void DoEvent();

	//�̵�
	void Move(const FInputActionValue& Value);

	//�ٶ󺸱�
	void Look(const FInputActionValue& Value);

	//�޸���
	void StartSprint();
	void EndSprint();

	//���¹̳� ������Ʈ
	void UpdateStamina();


	void DropWeapon();

	//���� ȣ�� �Լ� 1. normal Attack 2. sword attack
	void DoAttack();

	void PerformNormalAttack();

	void PerformSwordAttack();

	UFUNCTION(BlueprintCallable)
	void LineTrace();




};
