// Fill out your copyright notice in the Description page of Project Settings.


#include "HotbarWidget.h"
#include "Components/Image.h"
#include "Cupcake/PlayerSystem/InventoryComponent.h"
#include "Styling/SlateBrush.h"


void UHotbarWidget::SetUIImages(TArray<UImage*> NewUIImages)
{
	InventoryComponent = GetOwningPlayerPawn()->GetComponentByClass<UInventoryComponent>();
	
	UE_LOG(LogTemp, Warning, TEXT("Component: %p"), InventoryComponent);
	for (UImage* Image : NewUIImages)
	{
		if (Image != nullptr)
		{
			// Set the color and opacity of the image
			Image->SetColorAndOpacity(FLinearColor(0.2f, 0.2f, 0.2f, 0.0f));  // Dark grey with 100% opacity

			// Add the image to the array
			UIImages.Add(Image);
			UE_LOG(LogTemp, Display, TEXT("Added Image with dark grey color"));
		}
	}
	int ImageArraySize = UIImages.Max();
	UE_LOG(LogTemp, Display, TEXT("ImageArraySize: %d"), ImageArraySize);
}

void UHotbarWidget::SetUIBorders(TArray<UBorder*> NewUIBorder)
{
	for(UBorder* UBorder : NewUIBorder)
	{
		if(UBorder != nullptr)
		{
			UIBorder.Add(UBorder);
			UE_LOG(LogTemp, Display, TEXT("Added Border"));
			UBorder->SetBrushColor(FLinearColor(0.2f, 0.2f, 0.2f, 0.6f)); // Dark grey with 60% opacity
		}
	}
	UE_LOG(LogTemp, Display, TEXT("ImageArraySize: %d"), UIBorder.Num());
}

void UHotbarWidget::SetQuantity(TArray<UTextBlock*> NewQuantity)
{
	for(UTextBlock* TextBlock : NewQuantity)
	{
		Quantity.Add(TextBlock);
	}
	int TextBlockArray = Quantity.Max();
	UE_LOG(LogTemp, Display, TEXT("ImageArraySize: %d"), TextBlockArray);
}

void UHotbarWidget::UpdateHotbar(const TArray<AItem*>& Items)
{
	int NumItemsToUpdate = FMath::Clamp(Items.Num(), 0, UIImages.Num());
	UE_LOG(LogTemp, Display, TEXT("NumItemsToUpdate: %d"), NumItemsToUpdate);
	for(int i = 0; i < NumItemsToUpdate; i++)
	{
		if(Items[i] != nullptr)
		{
			// Update image
			if (UIImages.IsValidIndex(i) && UIImages[i] != nullptr)
			{
				FSlateBrush Brush;
				Brush.SetResourceObject(Items[i]->ItemThumbnail);
				Brush.ImageSize = FVector2D(Items[i]->ItemThumbnail->GetSizeX(), Items[i]->ItemThumbnail->GetSizeY());
				UIImages[i]->SetBrush(Brush);

				// Reset color and opacity to white and fully opaque
				UIImages[i]->SetColorAndOpacity(FLinearColor(1.0, 1.0, 1.0, 1.0));
			} else
			{
				UE_LOG(LogTemp, Warning, TEXT("Invalid Index for set image"));
			}

			// Update quantity
			if (Quantity.IsValidIndex(i) && Quantity[i] != nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("Before: %s"), *Quantity[i]->GetText().ToString());
				FText QuantityText = FText::AsNumber(Items[i]->Quantity);
				Quantity[i]->SetText(QuantityText);
				UE_LOG(LogTemp, Warning, TEXT("After: %s"), *Quantity[i]->GetText().ToString());
			} else
			{
				UE_LOG(LogTemp, Warning, TEXT("Invalid Index for set quantity"));
			}
		}
	}

}

void UHotbarWidget::HighlightImage(int32 ImageIndex)
{
	if (UIBorder.IsValidIndex(ImageIndex) && UIBorder[ImageIndex] != nullptr)
	{
		UIBorder[ImageIndex]->SetBrushColor(FLinearColor(HighlightColor)); // Dark grey with 60% opacity
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid or null UBorder at index %d"), ImageIndex);
	}
}

void UHotbarWidget::ResetHighlight(int32 ImageIndex)
{
	if (UIBorder.IsValidIndex(ImageIndex) && UIBorder[ImageIndex] != nullptr)
	{
		UIBorder[ImageIndex]->SetBrushColor(FLinearColor(0.2f, 0.2f, 0.2f, 0.6f)); // Reset to white
	}
}


void UHotbarWidget::HighLightUIItem(FKey KeyPressed)
{
	static int32 LastHighlightedIndex = -1; // Track the last highlighted item index
	UE_LOG(LogTemp, Warning, TEXT("Key Pressed: %s"), *KeyPressed.ToString());
	int ImageIndex = KeyBindings[KeyPressed];
	UE_LOG(LogTemp, Warning, TEXT("This input worked"));

	if (LastHighlightedIndex != -1)
	{
		ResetHighlight(LastHighlightedIndex);
	}
	
	if (UIBorder.IsValidIndex(ImageIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("Valid index: %d"), ImageIndex);
		HighlightImage(ImageIndex);
		EquippedItemID = InventoryComponent->InventoryItems[ImageIndex];
		UE_LOG(LogTemp, Warning, TEXT("Equipped item: %s"), *EquippedItemID->ItemName);
		LastHighlightedIndex = ImageIndex;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid index: %d"), ImageIndex);
	}
}



