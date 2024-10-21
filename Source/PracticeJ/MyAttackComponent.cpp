// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAttackComponent.h"
#include "MyHealthComponent.h"

// Sets default values for this component's properties
UMyAttackComponent::UMyAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMyAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMyAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMyAttackComponent::PerformAttack(AActor* Target)
{
	if (Target)
	{
		UMyHealthComponent* HealthComp = Target->FindComponentByClass<UMyHealthComponent>();
		if (HealthComp)
		{
			HealthComp->TakeDamage(AttackPower);
		}
	}
}

float UMyAttackComponent::GetAttackPower() const
{
	return AttackPower;
}

