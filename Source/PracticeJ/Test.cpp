// Fill out your copyright notice in the Description page of Project Settings.


#include "Test.h"
#include "MyCharacter.h"


/*ATest::ATest()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}*/

void ATest::OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Child override"));
	auto player = Cast<AMyCharacter>(OtherActor);

	if (player)
	{
		FName WeaponSocket(TEXT("RightHandWeaponSocket"));
		AttachToComponent(player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
	}
}
