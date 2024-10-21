// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyItem.h"
#include "MyWeapon.generated.h"

UCLASS()
class PRACTICEJ_API AMyWeapon : public AMyItem
{
	GENERATED_BODY()
	
	/*
public:
	AMyWeapon();

protected:
	virtual void BeginPlay() override;*/
	
public:
	FVector GetSocketLocation(const FName SocketName) const;

	float GetWeaponDamage();

private:
	//UPROPERTY(VisibleAnywhere, Category = "Weapon")
	//UStaticMeshComponent* WeaponMesh; // 무기 메쉬
	/*
	UPROPERTY(EditAnywhere)
	float WeaponDamage = 10.0f;

	
	*/
	/*UFUNCTION()
	void OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
		*/

	float WeaponDamage = 10.0f;

	//오버라이딩
	//UFUNCTION()
	void OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
