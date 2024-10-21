// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Delegates/Delegate.h"
#include "Components/ActorComponent.h"
#include "MyHealthComponent.generated.h"

// OnDamage 및 OnDeath 이벤트를 선언
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

	// 체력바 위젯 클래스
	//UPROPERTY(EditDefaultsOnly, Category = "UI")
	//TSubclassOf<class UMyHpWidget> HealthBarWidgetClass;

	// 위젯 컴포넌트
	//UPROPERTY(VisibleAnywhere, Category = "UI")
	//class UWidgetComponent* HealthBarWidgetComponent;

	
	UFUNCTION(BlueprintCallable, Category = "Health")
    void TakeDamage(float DamageAmount);

	//float GetHPRatio();

	// 체력바 업데이트
	//void UpdateHealthBar();

	//Delegate 데미지 받을 때 호출할 함수(이벤트 디스패쳐)
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnDamage OnDamage;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnDeath OnDeath;
};
