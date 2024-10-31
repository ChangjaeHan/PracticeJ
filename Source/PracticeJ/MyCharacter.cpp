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

	
	//애니메이션 클래스 지정
	/*
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimationInstance
	(TEXT("/Game/Assets/CharacterAnimation/BP_AnimMyCharacter.BP_AnimMyCharacter_C"));

	if (AnimationInstance.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimationInstance.Class);
	}*/
	



	GetCharacterMovement()->JumpZVelocity = JumpPower; // 점프 높이.
	GetCharacterMovement()->GravityScale = 1.5f;
	JumpMaxCount = 1; // 한번에 점프 가능한 횟수. 1이 기본 값.

	bHasWeapon = false;
	bHasDamaged = false;

	//공격컴포넌트 설정
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

	// 디버그 메시지를 5초간 표시합니다. 
	// -1 '키' 값 실행인자가 메시지가 업데이트되거나 새로고침되지 않도록 방지합니다.
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("We are using MyCharacter."));


	//인풋 매핑 컨텍스트
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
		//임시 이벤트 키 바인딩
		EnhancedInputComponent->BindAction(EventAction, ETriggerEvent::Completed, this, &AMyCharacter::DoEvent);

		//점프 바인딩
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//이동 바인딩
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyCharacter::Move);

		//바라보기 바인딩
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyCharacter::Look);

		//달리기 바인딩
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &AMyCharacter::StartSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AMyCharacter::EndSprint);


		//픽업 바인딩
		//EnhancedInputComponent->BindAction(PickupAction, ETriggerEvent::Triggered, this, &AMyCharacter::EquipWeapon);
		//드랍 바인딩
		EnhancedInputComponent->BindAction(DropAction, ETriggerEvent::Triggered, this, &AMyCharacter::DropWeapon);

		//공격 바인딩
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AMyCharacter::DoAttack);


		//상호작용 바인딩
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AMyCharacter::BeginInteract);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &AMyCharacter::EndInteract);

	}

}

void AMyCharacter::DropWeapon()
{
	if (bHasWeapon && CurrentWeapon)
	{
		// 무기 소켓에서 분리
		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		// 무기를 땅에 내려놓기 위한 위치 설정 (플레이어 앞에 놓기)
		FVector DropLocation = GetActorLocation() + GetActorForwardVector() * 200.0f;
		FRotator DropRotation = FRotator::ZeroRotator;

		// 무기 위치와 회전 설정
		CurrentWeapon->SetActorLocationAndRotation(DropLocation, DropRotation);

		// 무기의 소유권 제거
		CurrentWeapon->SetOwner(nullptr);

		// 플레이어가 더 이상 무기를 소지하지 않음
		bHasWeapon = false;
		CurrentWeapon = nullptr;

		// 디버그 메시지
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Weapon Dropped"));
	}
}


//Create linetrace
void AMyCharacter::PerformInteractionCheck()
{
	InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();
	FVector TraceStart{ GetPawnViewLocation()};
	FVector TraceEnd{ TraceStart + (GetViewRotation().Vector() * InteractionCheckDistance) };

	//뒤를 돌아봤을 때 trace가 앞으로 나가는 것을 방지, use Dot Product
	float LookDirection = FVector::DotProduct(GetActorForwardVector(), GetViewRotation().Vector());

	if (LookDirection > 0)
	{
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Blue, false, 1.0f);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		FHitResult TraceHit; //Hit 정보 받아옴

		if (GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
		{
			if (TraceHit.GetActor()->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
			{

				if (TraceHit.GetActor() != InteractionData.CurrentInteractable)
				{
					FoundInteractable(TraceHit.GetActor());
					return;
				}

				//계속 interaction 중인경우
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
		//크래시 나는걸 방지 (아이템이 사라졌는데 EndFocus하면 크래시)
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
		// 첫 번째 몬스터를 대상으로 데미지 입히기
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
	//스태미나 소모
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

	//범위 내 적에게 데미지
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
