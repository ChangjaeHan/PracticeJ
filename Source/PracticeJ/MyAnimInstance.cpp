// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UMyAnimInstance::UMyAnimInstance() //»ý¼ºÀÚ
{
	isFalling = false;

	/*static ConstructorHelpers::FObjectFinder<UAnimMontage> PUNCH_MONTAGE(TEXT(" / Game / Assets / CharacterAssets / Punch_Montage.Punch_Montage"));

	if (PUNCH_MONTAGE.Succeeded())
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("HEI"));
		}

		PunchMontage = PUNCH_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SLASH_MONTAGE(TEXT("/Game/Assets/CharacterAssets/Slash_Montage.Slash_Montage"));

	if (SLASH_MONTAGE.Succeeded())
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("HEI2"));
		}

		SlashMontage = SLASH_MONTAGE.Object;
	}*/
}


void UMyAnimInstance::PlayPunchMontage()
{
	if (!Montage_IsPlaying(PunchMontage))
	{
		Montage_Play(PunchMontage, 1.0f);
	}
	
}

void UMyAnimInstance::PlaySlashMontage()
{
	if (!Montage_IsPlaying(SlashMontage))
	{
		Montage_Play(SlashMontage, 1.0f);
	}
}

void UMyAnimInstance::PlayHitMontage()
{
	if (!Montage_IsPlaying(HitMontage))
	{
		Montage_Play(HitMontage, 1.0f);
	}
}

void UMyAnimInstance::PlayDieMontage()
{
	if (!Montage_IsPlaying(DieMontage))
	{
		Montage_Play(DieMontage, 1.0f);
	}
}


void UMyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(GetOwningActor());

	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		Velocity = Movement->Velocity;
		MoveSpeed = Velocity.Size2D();
		ZSpeed = Velocity.Z;
		isFalling = Movement->IsFalling();
	}
}
