// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionTestActor.h"

// Sets default values
AInteractionTestActor::AInteractionTestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");

	SetRootComponent(Mesh);


}

// Called when the game starts or when spawned
void AInteractionTestActor::BeginPlay()
{
	Super::BeginPlay();

	InteractableData = InstanceInteractableData;
	
}

// Called every frame
void AInteractionTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractionTestActor::BeginFocus()
{
	if (Mesh)
	{
		Mesh->SetRenderCustomDepth(true);
	}
}

void AInteractionTestActor::EndFocus()
{
	if (Mesh)
	{
		Mesh->SetRenderCustomDepth(false);
	}
}

void AInteractionTestActor::BeginInteract()
{
	UE_LOG(LogTemp, Warning, TEXT("Calling BeginInteract override on interface test actor."));

}

void AInteractionTestActor::EndInteract()
{
	UE_LOG(LogTemp, Warning, TEXT("Calling EndInteract override on interface test actor."));

}

void AInteractionTestActor::Interact(AMyCharacter* PlayerCharacter)
{
	UE_LOG(LogTemp, Warning, TEXT("Calling Interact override on interface test actor."));

}

