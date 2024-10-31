// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionWidget.generated.h"


class AMyCharacter;
struct FInteractableData;
class UTextBlock;
class UProgressBar;
/**
 * 
 */
UCLASS()
class PRACTICEJ_API UInteractionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, Category = "Interaction Widget | Player Reference")
	AMyCharacter* PlayerReference;

	void UpdateWidget(const FInteractableData* InteractableData) const;

protected:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data") //meta -> create graphical elements for this widget
	UTextBlock* NameText;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data") //meta -> create graphical elements for this widget
	UTextBlock* ActionText;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data") //meta -> create graphical elements for this widget
	UTextBlock* QuantityText;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data") //meta -> create graphical elements for this widget
	UTextBlock* KeyPressText;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data") //meta -> create graphical elements for this widget
	UProgressBar* InteractionProgressBar;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data") //meta -> create graphical elements for this widget
	float CurrentInteractionDuration;

	UFUNCTION(Category = "Interaction Widget | Interactable Data")
	float UpdateInteractionProgress();

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
};
