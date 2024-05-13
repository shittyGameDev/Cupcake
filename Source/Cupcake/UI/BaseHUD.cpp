// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseHUD.h"
#include "MainMenu.h"
#include "InteractionWidget.h"
#include "PickupWidget.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Cupcake/Items/BaseItem.h"
#include "Cupcake/PlayerSystem/NewInventoryComponent.h"


ABaseHUD::ABaseHUD()
{
}

void ABaseHUD::BeginPlay()
{
	Super::BeginPlay();

	if (MainMenuClass)
	{
		MainMenuWidget = CreateWidget<UMainMenu>(GetWorld(), MainMenuClass);
		MainMenuWidget->AddToViewport(5);
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
		//Tydligen har Collapsed en bättre påverkan på performance än Hidden
	}

	if (InteractionWidgetClass)
	{
		InteractionWidget = CreateWidget<UInteractionWidget>(GetWorld(), InteractionWidgetClass);
		InteractionWidget->AddToViewport(-1);
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
		//Tydligen har Collapsed en bättre påverkan på performance än Hidden
	}

	PlayerCharacter = Cast<ACupcakeCharacter>(GetOwningPawn());
	if (PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("I found the player"));
		PlayerCharacter->GetInventory()->OnPickup.AddDynamic(this, &ABaseHUD::DisplayPickup);
		PlayerCharacter->GetInventory()->OnRemoveItem.AddDynamic(this, &ABaseHUD::DisplayDrop);
		
	}
}

void ABaseHUD::DisplayMenu()
{
	if (MainMenuWidget)
	{
		bIsMenuVisible = true;
		//OpenMenu.Broadcast();
		AnimateOpenMenu();
		//MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ABaseHUD::HideMenu()
{
	if (MainMenuWidget)
	{
		//OpenMenu.Broadcast();
		bIsMenuVisible = false;
		AnimateCloseMenu();
		//MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ABaseHUD::ToggleMenu()
{
	if (bIsMenuVisible)
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
	PickupWidget = CreateWidget<UPickupWidget>(GetWorld(), PickupWidgetClass);
	if (PickupWidget)
	{
		PickupWidget->AddToViewport(5);

		if (ItemRef && ItemRef->AssetData.Icon && ItemRef->Quantity)
		{
			FSlateBrush NewBrush;
			NewBrush.SetResourceObject(ItemRef->AssetData.Icon);
			NewBrush.ImageSize = PickupWidget->ItemIcon->GetBrush().ImageSize;
			NewBrush.DrawAs = ESlateBrushDrawType::Image;

			PickupWidget->ItemIcon->SetBrush(NewBrush);
			PickupWidget->ItemQuantity->SetText(FText::AsNumber(ItemRef->Quantity));
			PickupWidget->PlusMinus->SetText(FText::FromString(TEXT("+")));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create the widget"));
	}
}

void ABaseHUD::DisplayDrop(UBaseItem* ItemRef)
{
	PickupWidget = CreateWidget<UPickupWidget>(GetWorld(), PickupWidgetClass);
	if (PickupWidget)
	{
		PickupWidget->AddToViewport(5);

		if (ItemRef && ItemRef->AssetData.Icon && ItemRef->Quantity)
		{
			FSlateBrush NewBrush;
			NewBrush.SetResourceObject(ItemRef->AssetData.Icon);
			NewBrush.ImageSize = PickupWidget->ItemIcon->GetBrush().ImageSize;
			NewBrush.DrawAs = ESlateBrushDrawType::Image;

			PickupWidget->ItemIcon->SetBrush(NewBrush);
			PickupWidget->ItemQuantity->SetText(FText::AsNumber(ItemRef->Quantity));
			PickupWidget->PlusMinus->SetText(FText::FromString(TEXT("-")));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create the widget"));
	}
}

void ABaseHUD::ShowInteractionWidget()
{
	if (InteractionWidget)
	{
		//InteractionWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ABaseHUD::HideInteractionWidget()
{
	if (InteractionWidget)
	{
		//InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
		AnimateCloseInteractionWidget();
	}
}

void ABaseHUD::UpdateInteractionWidget(const FInteractableData* InteractableData)
{
	UE_LOG(LogTemp, Warning, TEXT("Consider it updated"));
	if (InteractionWidget)
	{
		if (InteractionWidget->GetVisibility() == ESlateVisibility::Collapsed)
		{
			//InteractionWidget->SetVisibility(ESlateVisibility::Visible);
			AnimateOpenInteractionWidget();
			//InteractionWidget->InteractionProgressBar->SetPercent(0);
		}
		InteractionWidget->UpdateWidget(InteractableData);
	}
}

void ABaseHUD::UpdateInteractionProgress(float Progress) const
{
	if (InteractionWidget->InteractionProgressBar)
	{
		InteractionWidget->InteractionProgressBar->SetPercent(Progress);
		if (Progress == 1)
		{
			InteractionWidget->InteractionProgressBar->SetPercent(0);
		}
	}
}
