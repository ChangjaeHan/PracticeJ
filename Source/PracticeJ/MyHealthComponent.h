// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Delegates/Delegate.h"
#include "Components/ActorComponent.h"
#include "MyHealthComponent.generated.h"

// OnDamage �� OnDeath �̺�Ʈ�� ����
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDamage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PRACTICEJ_API UMyHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	float CurrentHealth;

	// ü�¹� ���� Ŭ����
	//UPROPERTY(EditDefaultsOnly, Category = "UI")
	//TSubclassOf<class UMyHpWidget> HealthBarWidgetClass;

	// ���� ������Ʈ
	//UPROPERTY(VisibleAnywhere, Category = "UI")
	//class UWidgetComponent* HealthBarWidgetComponent;

	
	UFUNCTION(BlueprintCallable, Category = "Health")
    void TakeDamage(float DamageAmount);

	//float GetHPRatio();

	// ü�¹� ������Ʈ
	//void UpdateHealthBar();

	//Delegate ������ ���� �� ȣ���� �Լ�(�̺�Ʈ ������)
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnDamage OnDamage;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnDeath OnDeath;
};
