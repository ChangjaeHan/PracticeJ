// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "MyWeaponPickup.generated.h"

UCLASS()
class PRACTICEJ_API AMyWeaponPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyWeaponPickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Interacted();

	bool GetActive();

private:

	UPROPERTY()
	bool Active;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	class UStaticMeshComponent* Mesh;


};
