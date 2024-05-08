// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseHUD.h"
#include "MainMenu.h"
#include "InteractionWidget.h"
#include "PickupWidget.h"
#include "Components/ProgressBar.h"
#include "Cupcake/Items/BaseItem.h"
#include "Cupcake/PlayerSystem/NewInventoryComponent.h"


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

	PlayerCharacter = Cast<ACupcakeCharacter>(GetOwningPawn());
	if(PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("I found the player"));
		PlayerCharacter->GetInventory()->OnPickup.AddDynamic(this, &ABaseHUD::DisplayPickup);
	}
}

void ABaseHUD::DisplayMenu()
{
	if(MainMenuWidget)
	{
		bIsMenuVisible = true;
		OpenMenu.Broadcast();
		AnimateOpenMenu();
		//MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ABaseHUD::HideMenu()
{
	if(MainMenuWidget)
	{
		OpenMenu.Broadcast();
		bIsMenuVisible = false;
		AnimateCloseMenu();
		//MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ABaseHUD::ToggleMenu()
{
	if(bIsMenuVisible)
	{
		HideMenu();
		const FInputModeGameOnly InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
	}
	else
	{
		DisplayMenu();
		const FInputModeGameAndUI InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
	}
}

void ABaseHUD::DisplayPickup(UBaseItem* ItemRef)
{
	// Create the pickup widget and add it to the viewport with a high priority.
	PickupWidget = CreateWidget<UPickupWidget>(GetWorld(), PickupWidgetClass);
	if (PickupWidget)
	{
		PickupWidget->AddToViewport(5);

		if (ItemRef && ItemRef->AssetData.Icon) // Ensure the item and icon are valid.
			{
			FSlateBrush NewBrush;
			NewBrush.SetResourceObject(ItemRef->AssetData.Icon);
			NewBrush.ImageSize = PickupWidget->ItemIcon->GetBrush().ImageSize;
			NewBrush.DrawAs = ESlateBrushDrawType::Image; // Specify how to draw this brush.

			PickupWidget->ItemIcon->SetBrush(NewBrush);
			}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create the widget"));
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
	UE_LOG(LogTemp, Warning, TEXT("Consider it updated"));
	if(InteractionWidget)
	{
		if(InteractionWidget->GetVisibility() == ESlateVisibility::Collapsed)
		{
			InteractionWidget->SetVisibility(ESlateVisibility::Visible);
			//InteractionWidget->InteractionProgressBar->SetPercent(0);
		}
		InteractionWidget->UpdateWidget(InteractableData);
	}
}

void ABaseHUD::UpdateInteractionProgress(float Progress) const
{
	if(InteractionWidget->InteractionProgressBar)
	{
		InteractionWidget->InteractionProgressBar->SetPercent(Progress);
		if(Progress == 1)
		{
			InteractionWidget->InteractionProgressBar->SetPercent(0);
		}
	}
}




