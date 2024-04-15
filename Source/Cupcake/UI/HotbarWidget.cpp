// Fill out your copyright notice in the Description page of Project Settings.


#include "HotbarWidget.h"
#include "Components/Image.h"
#include "Styling/SlateBrush.h"



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

void UHotbarWidget::SetUIImages(TArray<UImage*> NewUIImages)
{
	for(UImage* UImage : NewUIImages)
	{
		UIImages.Add(UImage);
		UE_LOG(LogTemp, Display, TEXT("Added Image"));
		
	}
	int ImageArraySize = UIImages.Max();
	UE_LOG(LogTemp, Display, TEXT("ImageArraySize: %d"), ImageArraySize);
}

void UHotbarWidget::HighlightImage(int32 ImageIndex)
{
	if (UIImages.IsValidIndex(ImageIndex) && UIImages[ImageIndex] != nullptr)
	{
		// Apply a highlight effect, e.g., change tint color
		FSlateBrush NewBrush = UIImages[ImageIndex]->Brush;
		NewBrush.TintColor = FSlateColor(FLinearColor(1.0, 0.0, 0.0, 1.0)); // Red tint
		UIImages[ImageIndex]->SetBrush(NewBrush);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid or null UIImage at index %d"), ImageIndex);
	}
}

void UHotbarWidget::ResetHighlight(int32 ImageIndex)
{
	if (UIImages.IsValidIndex(ImageIndex) && UIImages[ImageIndex] != nullptr)
	{
		// Reset the tint color or brush to default
		FSlateBrush DefaultBrush = UIImages[ImageIndex]->Brush;
		DefaultBrush.TintColor = FSlateColor(FLinearColor(1.0, 1.0, 1.0, 1.0)); // Default white
		UIImages[ImageIndex]->SetBrush(DefaultBrush);
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
	
	if (UIImages.IsValidIndex(ImageIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("Valid index: %d"), ImageIndex);
		HighlightImage(ImageIndex);
		LastHighlightedIndex = ImageIndex;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid index: %d"), ImageIndex);
	}
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
