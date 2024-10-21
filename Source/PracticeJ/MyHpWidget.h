// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyHpWidget.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEJ_API UMyHpWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:

    UPROPERTY(meta = (BindWidget))
    class UProgressBar* HealthBar;

    // 체력 값 업데이트 함수
    UFUNCTION(BlueprintCallable, Category = "UI")
    void SetHealthPercent(float Percent);

};
