// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"
#include "MainMenu.h"
#include "InteractionWidget.h"

AMyHUD::AMyHUD()
{
}

void AMyHUD::BeginPlay()
{
	Super::BeginPlay();

	if (MainMenuClass)
	{
		MainMenuWidget = CreateWidget<UMainMenu>(GetWorld(), MainMenuClass);
		MainMenuWidget->AddToViewport(5);
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed); //collapsed->hidden은 보이지않을때도 running이므로 비효율
	}

	if (InteractionWidgetClass)
	{
		InteractionWidget = CreateWidget<UInteractionWidget>(GetWorld(), MainMenuClass);
		InteractionWidget->AddToViewport(-1);
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed); //collapsed->better for performance
	}

}

void AMyHUD::DisplayMenu()
{
	if (MainMenuWidget)
	{
		bIsMenuVisible = true;
		MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMyHUD::HideMenu()
{
	if (MainMenuWidget)
	{
		bIsMenuVisible = false;
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AMyHUD::ShowInteractionWidget() const
{
	if (InteractionWidget)
	{
		MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMyHUD::HideInteractionWidget() const
{
	if (InteractionWidget)
	{
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AMyHUD::UpdateInteractionWidget(const FInteractableData* InteractableData) const
{
	if (InteractionWidget)
	{
		if (InteractionWidget->GetVisibility() == ESlateVisibility::Collapsed)
		{
			InteractionWidget->SetVisibility(ESlateVisibility::Visible);
		}

		InteractionWidget->UpdateWidget(InteractableData);
	}
}


