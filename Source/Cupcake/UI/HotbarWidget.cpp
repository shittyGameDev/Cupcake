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
			FSlateBrush Brush;
			Brush.SetResourceObject(Items[i]->ItemThumbnail);
			Brush.ImageSize = FVector2D(Items[i]->ItemThumbnail->GetSizeX(), Items[i]->ItemThumbnail->GetSizeY());
			UIImages[i]->SetBrush(Brush);
			UE_LOG(LogTemp, Display, TEXT("Added Image"));
		}
	}
}

void UHotbarWidget::SetUIImages(TArray<UImage*> NewUIImages)
{
	for(UImage* UImage : NewUIImages)
	{
		UIImages.Add(UImage);
		UE_LOG(LogTemp, Display, TEXT("Added Image"));
		int ImageArraySize = UIImages.Max();
		UE_LOG(LogTemp, Display, TEXT("ImageArraySize: %d"), ImageArraySize);
	}
}
