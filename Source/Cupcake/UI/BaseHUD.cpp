// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseHUD.h"
#include "MainMenu.h"
#include "InteractionWidget.h"
#include "Components/ProgressBar.h"


ABaseHUD::ABaseHUD()
{
}

void ABaseHUD::BeginPlay()
{
	Super::BeginPlay();

	if(MainMenuClass)
	{
		MainMenuWidget = CreateWidget<UMainMenu>(GetWorld(), MainMenuClass);
		MainMenuWidget->AddToViewport(5);
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed); //Tydligen har Collapsed en bättre påverkan på performance än Hidden
	}

	if(InteractionWidgetClass)
	{
		InteractionWidget = CreateWidget<UInteractionWidget>(GetWorld(), InteractionWidgetClass);
		InteractionWidget->AddToViewport(-1);
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed); //Tydligen har Collapsed en bättre påverkan på performance än Hidden
	}
}

void ABaseHUD::DisplayMenu()
{
	if(MainMenuWidget)
	{
		bIsMenuVisible = true;
		MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ABaseHUD::HideMenu()
{
	if(MainMenuWidget)
	{
		bIsMenuVisible = false;
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ABaseHUD::ToggleMenu()
{
	if(bIsMenuVisible)
	{
		HideMenu();

		const FInputModeGameOnly InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(false);
	}
	else
	{
		DisplayMenu();
		const FInputModeGameAndUI InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(true);
	}
}

void ABaseHUD::ShowInteractionWidget() const
{
	if(InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ABaseHUD::HideInteractionWidget() const
{
	if(InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ABaseHUD::UpdateInteractionWidget(const FInteractableData* InteractableData) const
{
	if(InteractionWidget)
	{
		if(InteractionWidget->GetVisibility() == ESlateVisibility::Collapsed)
		{
			InteractionWidget->SetVisibility(ESlateVisibility::Visible);
			InteractionWidget->InteractionProgressBar->SetPercent(0);
		}
		InteractionWidget->UpdateWidget(InteractableData);
	}
}

void ABaseHUD::UpdateInteractionProgress(float Progress) const
{
	UE_LOG(LogTemp, Warning, TEXT("This was reached"));
	if(InteractionWidget->InteractionProgressBar)
	{
		InteractionWidget->InteractionProgressBar->SetPercent(Progress);
		UE_LOG(LogTemp, Warning, TEXT("Progress: %f"), Progress);
		if(Progress == 1)
		{
			InteractionWidget->InteractionProgressBar->SetPercent(0);
		}
	}
}




