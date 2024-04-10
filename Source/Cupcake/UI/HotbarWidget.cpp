// Fill out your copyright notice in the Description page of Project Settings.


#include "HotbarWidget.h"
#include "Components/Image.h"


void UHotbarWidget::UpdateHotbar(const TArray<AItem*>& Items)
{
	// Update images and quantity
	for (int i = 0; i < UIImages.Max(); i++)
	{
		UIImages[i] = Items[i]->ItemThumbnail;
	}
	
}

void UHotbarWidget::SetUIImages(TArray<UImage*> NewUIImages)
{
	for(UImage* UImage : NewUIImages)
	{
		UIImages.Add(UImage);
		UE_LOG(LogTemp, Display, TEXT("Added Image"));
	}
}
