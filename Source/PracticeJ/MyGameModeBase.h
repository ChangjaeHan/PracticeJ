// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameModeBase.generated.h"

#define ABCHECK(Expr, ...) { if (!(Expr)) { ABLOG(Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__;} }


/**
 * 
 */
UCLASS()
class PRACTICEJ_API AMyGameModeBase : public AGameModeBase
{

	GENERATED_BODY()
	
	virtual void StartPlay() override;
};
