// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWeapon.h"
#include "MyCharacter.h"
#include "Components/StaticMeshComponent.h"
//#include "Engine/Engine.h"

//AMyWeapon::AMyWeapon()
//{
	//WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	//WeaponMesh = PickupMesh;
	//PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	//RootComponent = PickupMesh;

	//PickupMesh->SetCollisionProfileName(TEXT("MyCollectible"));
	//PickupMesh->OnComponentBeginOverlap.AddDynamic(this, &AMyWeapon::OnPlayerBeginOverlap);
	//RootComponent = WeaponMesh;

	//WeaponMesh->SetCollisionProfileName(TEXT("MyCollectible"));

	//WeaponMesh->OnComponentBeginOverlap.AddDynamic(this, &AMyWeapon::OnPlayerBeginOverlap);
//}

/*
void AMyWeapon::BeginPlay()
{
	Super::BeginPlay();
}*/


FVector AMyWeapon::GetSocketLocation(const FName SocketName) const
{
	if (PickupMesh)
	{
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, TEXT("Getting SocketLocaiton.."));
		return PickupMesh->GetSocketLocation(SocketName);
	}
	else
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, TEXT("Not Found Location"));
		return FVector::ZeroVector;
	}
}

float AMyWeapon::GetWeaponDamage()
{
	return WeaponDamage;
}

void AMyWeapon::OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Child override"));
	auto player = Cast<AMyCharacter>(OtherActor);

	if (player && !(player->bHasWeapon))
	{
		FName WeaponSocket(TEXT("RightHandWeaponSocket"));
		
		this->AttachToComponent(player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		player->bHasWeapon = true;
		player->CurrentWeapon = this;
		this->SetOwner(player);
	}
}
