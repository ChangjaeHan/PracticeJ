// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyItem.h"
#include "Test.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEJ_API ATest : public AMyItem
{
	GENERATED_BODY()


	//오버라이딩
	void OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


};
