// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEJ_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	

public:
	UMyAnimInstance(); //생성자


public:
	void PlayPunchMontage();

	void PlaySlashMontage();

	void PlayHitMontage();

	void PlayDieMontage();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackMontage", Meta = (AllowPrivateAccess = true))
	UAnimMontage* PunchMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackMontage", Meta = (AllowPrivateAccess = true))
	UAnimMontage* SlashMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitMontage", Meta = (AllowPrivateAccess = true))
	UAnimMontage* HitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitMontage", Meta = (AllowPrivateAccess = true))
	UAnimMontage* DieMontage;



protected:
	virtual void NativeInitializeAnimation() override; //애니메이션 생성시 호출
	virtual void NativeUpdateAnimation(float DeltaSeconds) override; //프레임마다 호출

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	class ACharacter* Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	class UCharacterMovementComponent* Movement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	FVector Velocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	float MoveSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	bool isFalling = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	float ZSpeed;
};
