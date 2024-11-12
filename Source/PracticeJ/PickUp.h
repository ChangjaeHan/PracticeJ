// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionInterface.h"
#include "PickUp.generated.h"

class UDataTable;
class UItemBase;

UCLASS()
class PRACTICEJ_API APickUp : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	

	APickUp();

	void InitializePickup(const TSubclassOf<UItemBase> BaseClass, const int32 InQuantity);
	void InitializeDrop(UItemBase* ItemToDrop, const int32 InQuantity);
	
	FORCEINLINE UItemBase* GetItemData() { return ItemReference; };

	virtual void BeginFocus() override;
	virtual void EndFocus() override;

protected:

	UPROPERTY(VisibleAnywhere, Category = "Pickup | Components")
	UStaticMeshComponent* PickupMesh;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	UDataTable* ItemDataTable;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	FName DesiredItemID;

	UPROPERTY(VisibleAnywhere, Category = "Pickup | Item Reference")
	UItemBase* ItemReference;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	int32 ItemQuantity;

	UPROPERTY(VisibleInstanceOnly, Category = "Pickup | Interaction")
	FInteractableData InstanceInteractableData;
	
	virtual void BeginPlay() override;

public:
	
	virtual void Interact(AMyCharacter* PlayerCharacter) override;
	void UpdateInteractableData();


	void TakePickup(const AMyCharacter* Taker);


#if WITH_EDITOR //if you compiling in the editor, it will build this otherwise skip
	//아이템 메시를 따로 지정해주지 않아도 알아서 에디터에서 바뀌게끔 
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif


};
