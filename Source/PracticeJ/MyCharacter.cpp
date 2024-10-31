// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "MyMonster.h"
#include "MyWeapon.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyAnimInstance.h"
#include "MyHUD.h"
#include "DrawDebugHelpers.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseEyeHeight = 72.0f;

	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CamerBoom"));
	CameraArm->SetupAttachment(GetMesh());
	//CameraArm->SetupAttachment(GetCapsuleComponent());
	CameraArm->SetRelativeLocation(FVector(0.0f, 0.0f, BaseEyeHeight));
	CameraArm->TargetArmLength = 600.f;
	CameraArm->bUsePawnControlRotation = true;

	/*
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));
	FirstPersonCamera->SetRelativeLocation(FVector(0.0f, 0.0f, 20.0f + BaseEyeHeight));
	FirstPersonCamera->bUsePawnControlRotation = true;
	*/


	ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
	ThirdPersonCamera->SetupAttachment(CameraArm, USpringArmComponent::SocketName);
	ThirdPersonCamera->SetRelativeLocation(FVector(0.0f, 0.0f, BaseEyeHeight));
	ThirdPersonCamera->bUsePawnControlRotation = false;

	
	//�ִϸ��̼� Ŭ���� ����
	/*
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimationInstance
	(TEXT("/Game/Assets/CharacterAnimation/BP_AnimMyCharacter.BP_AnimMyCharacter_C"));

	if (AnimationInstance.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimationInstance.Class);
	}*/
	



	GetCharacterMovement()->JumpZVelocity = JumpPower; // ���� ����.
	GetCharacterMovement()->GravityScale = 1.5f;
	JumpMaxCount = 1; // �ѹ��� ���� ������ Ƚ��. 1�� �⺻ ��.

	bHasWeapon = false;
	bHasDamaged = false;

	//����������Ʈ ����
	AttackComponent = CreateDefaultSubobject<UMyAttackComponent>(TEXT("AttackComponent"));

	
	InteractionCheckFrequency = 0.1;
	InteractionCheckDistance = 350.0f;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	HUD = Cast<AMyHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	
	
	check(GEngine != nullptr);

	// ����� �޽����� 5�ʰ� ǥ���մϴ�. 
	// -1 'Ű' �� �������ڰ� �޽����� ������Ʈ�ǰų� ���ΰ�ħ���� �ʵ��� �����մϴ�.
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("We are using MyCharacter."));


	//��ǲ ���� ���ؽ�Ʈ
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller)) 
	{
		
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (!InputMapping.IsNull())
			{
				UInputMappingContext* MappingContext = InputMapping.LoadSynchronous();
				Subsystem->AddMappingContext(MappingContext, 0 /*Priority*/);
			}
		}

	}


}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateStamina();




	if (GetWorld()->TimeSince(InteractionData.LastInteractionCheckTime) > InteractionCheckFrequency)
	{
		PerformInteractionCheck();
	}

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//�ӽ� �̺�Ʈ Ű ���ε�
		EnhancedInputComponent->BindAction(EventAction, ETriggerEvent::Completed, this, &AMyCharacter::DoEvent);

		//���� ���ε�
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//�̵� ���ε�
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyCharacter::Move);

		//�ٶ󺸱� ���ε�
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyCharacter::Look);

		//�޸��� ���ε�
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &AMyCharacter::StartSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AMyCharacter::EndSprint);


		//�Ⱦ� ���ε�
		//EnhancedInputComponent->BindAction(PickupAction, ETriggerEvent::Triggered, this, &AMyCharacter::EquipWeapon);
		//��� ���ε�
		EnhancedInputComponent->BindAction(DropAction, ETriggerEvent::Triggered, this, &AMyCharacter::DropWeapon);

		//���� ���ε�
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AMyCharacter::DoAttack);


		//��ȣ�ۿ� ���ε�
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AMyCharacter::BeginInteract);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &AMyCharacter::EndInteract);

	}

}

void AMyCharacter::DropWeapon()
{
	if (bHasWeapon && CurrentWeapon)
	{
		// ���� ���Ͽ��� �и�
		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		// ���⸦ ���� �������� ���� ��ġ ���� (�÷��̾� �տ� ����)
		FVector DropLocation = GetActorLocation() + GetActorForwardVector() * 200.0f;
		FRotator DropRotation = FRotator::ZeroRotator;

		// ���� ��ġ�� ȸ�� ����
		CurrentWeapon->SetActorLocationAndRotation(DropLocation, DropRotation);

		// ������ ������ ����
		CurrentWeapon->SetOwner(nullptr);

		// �÷��̾ �� �̻� ���⸦ �������� ����
		bHasWeapon = false;
		CurrentWeapon = nullptr;

		// ����� �޽���
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Weapon Dropped"));
	}
}


//Create linetrace
void AMyCharacter::PerformInteractionCheck()
{
	InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();
	FVector TraceStart{ GetPawnViewLocation()};
	FVector TraceEnd{ TraceStart + (GetViewRotation().Vector() * InteractionCheckDistance) };

	//�ڸ� ���ƺ��� �� trace�� ������ ������ ���� ����, use Dot Product
	float LookDirection = FVector::DotProduct(GetActorForwardVector(), GetViewRotation().Vector());

	if (LookDirection > 0)
	{
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Blue, false, 1.0f);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		FHitResult TraceHit; //Hit ���� �޾ƿ�

		if (GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
		{
			if (TraceHit.GetActor()->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
			{

				if (TraceHit.GetActor() != InteractionData.CurrentInteractable)
				{
					FoundInteractable(TraceHit.GetActor());
					return;
				}

				//��� interaction ���ΰ��
				if (TraceHit.GetActor() == InteractionData.CurrentInteractable)
				{
					return;
				}
			}
		}
	}

	NoInteractableFound();

}

void AMyCharacter::FoundInteractable(AActor* NewInteractable)
{
	if (IsInteracting()) 
	{
		EndInteract();
	}

	if (InteractionData.CurrentInteractable)
	{
		TargetInteractable = InteractionData.CurrentInteractable;
		TargetInteractable->EndFocus();
	}

	InteractionData.CurrentInteractable = NewInteractable;
	TargetInteractable = NewInteractable;

	HUD->UpdateInteractionWidget(&TargetInteractable->InteractableData);



	TargetInteractable->BeginFocus();
}

void AMyCharacter::NoInteractableFound()
{
	if (IsInteracting())
	{
		GetWorldTimerManager().ClearTimer(TimerHandleInteraction);
	}

	if (InteractionData.CurrentInteractable)
	{
		//ũ���� ���°� ���� (�������� ������µ� EndFocus�ϸ� ũ����)
		if (IsValid(TargetInteractable.GetObject())) 
		{
			TargetInteractable->EndFocus();
		}

		//Hide Interaction Widget on the HUD
		HUD->HideInteractionWidget();

		InteractionData.CurrentInteractable = nullptr;
		TargetInteractable = nullptr;
	}
}

void AMyCharacter::BeginInteract()
{
	//Verify nothing has changed with the interactable state since beginning interaction
	PerformInteractionCheck();

	if (InteractionData.CurrentInteractable)
	{
		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->BeginInteract();

			if (FMath::IsNearlyZero(TargetInteractable->InteractableData.InteractionDuration, 0.1f))
			{
				Interact();
			}
			else
			{ //handle the delay
				GetWorldTimerManager().SetTimer(TimerHandleInteraction,
					this,
					&AMyCharacter::Interact,
					TargetInteractable->InteractableData.InteractionDuration,
					false);
			}
		}
	}
}

void AMyCharacter::EndInteract()
{

	GetWorldTimerManager().ClearTimer(TimerHandleInteraction);
	
	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->EndInteract();
	}
}

void AMyCharacter::Interact()
{
	GetWorldTimerManager().ClearTimer(TimerHandleInteraction);

	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->Interact(this);
	}
}


void AMyCharacter::DoEvent()
{
	TArray<AActor*> FoundMonsters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMyMonster::StaticClass(), FoundMonsters);

	if (FoundMonsters.Num() > 0)
	{
		// ù ��° ���͸� ������� ������ ������
		AActor* MyMonster = FoundMonsters[0];
		if (MyMonster)
		{
			UMyHealthComponent* HealthComp = MyMonster->FindComponentByClass<UMyHealthComponent>();
			if (HealthComp)
			{
				float DamageAmount = 20.0f;
				HealthComp->TakeDamage(DamageAmount);
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Attack Monster@@"));
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("No Monster to Attack??"));
		}
	}

}



void AMyCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMyCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AMyCharacter::StartSprint()
{
	if (bHasStamina)
	{
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;

		if (GetVelocity().Size() >= 0.5)
		{
			bIsRunning = true;
		}
		else
		{
			bIsRunning = false;
		}
	}
}

void AMyCharacter::EndSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	bIsRunning = false;
}

void AMyCharacter::UpdateStamina()
{
	//���¹̳� �Ҹ�
	if (bIsRunning)
	{
		CurStamina -= StaminaDrainTime;
		CurRefillDelayTime = DelayBeforeRefill;
	}

	if (!bIsRunning && CurStamina < MaxStamina)
	{
		CurRefillDelayTime--;
		if (CurRefillDelayTime <= 0)
		{
			CurStamina += StaminaRefillTime;
		}
	}

	if (CurStamina <= 0)
	{
		bHasStamina = false;
		EndSprint();
	}
	else
	{
		bHasStamina = true;
	}
}



void AMyCharacter::DoAttack()
{

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("You are punching"));

	if (!bHasWeapon)
	{
		PerformNormalAttack();
	}
	else
	{
		PerformSwordAttack();
	}

	//TODO:
	
}

void AMyCharacter::PerformNormalAttack()
{
	auto Anim = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());

	if (Anim == nullptr) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Puncing anim is not"));
		
		return;
	}
	if (Anim && !bIsAttacking)
	{
		Anim->PlayPunchMontage();
		bIsAttacking = true;
	}
	bIsAttacking = false;
}

void AMyCharacter::PerformSwordAttack()
{
	auto Anim = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());

	if (Anim == nullptr) {
		if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Puncing anim is not"));
		
		return;
	}
	if (Anim && !bIsAttacking)
	{
		Anim->PlaySlashMontage();
		bIsAttacking = true;
	}
	bIsAttacking = false;
}

void AMyCharacter::LineTrace()
{

	//���� �� ������ ������
	FVector StartLocation = CurrentWeapon->GetSocketLocation(FName("Start"));
	FVector EndLocation = CurrentWeapon->GetSocketLocation(FName("End"));

	FHitResult HitResult;
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this);

	//LineTrace
	GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, TraceParams);

	//Debug

	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 1, 0, 1);


	if (HitResult.bBlockingHit)
	{
		AActor* ActorHit = HitResult.GetActor();
		
		UMyHealthComponent* HealthComp = ActorHit->FindComponentByClass<UMyHealthComponent>();
		if (HealthComp && !bHasDamaged)
		{
			float DamageAmount = CurrentWeapon->GetWeaponDamage();
			HealthComp->TakeDamage(DamageAmount);
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Attack Monster@@"));
		}
	}
	else
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("No Damage to Monster"));
	}
}
