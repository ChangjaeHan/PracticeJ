// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyHealthComponent.h"
#include "MyAnimInstance.h"
#include "MyMonster.generated.h"

UCLASS()
class PRACTICEJ_API AMyMonster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyMonster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Components")
	UMyHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, Category = UI)
	class UWidgetComponent* HealthBarWidgetComponent;


	void UpdateHealthBar();

	UFUNCTION()
	void HandleOnDamage();

	UFUNCTION()
	void HandleOnDeath();

};
