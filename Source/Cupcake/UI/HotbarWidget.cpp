// Fill out your copyright notice in the Description page of Project Settings.


#include "HotbarWidget.h"

#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Image.h"


void UHotbarWidget::UpdateHotbar(const TArray<AItem*>& Items)
{

	UWidget* FoundWidget = GetWidgetFromName(TEXT("ItemsContainer"));
	if (FoundWidget == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetWidgetFromName returned nullptr. 'ItemsContainer' not found."));
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("GetWidgetFromName succeeded. Found 'ItemsContainer'."));
	}
	
	UE_LOG(LogTemp, Display, TEXT("Updating: "))
	UHorizontalBox* ItemsContainer = Cast<UHorizontalBox>(GetWidgetFromName(TEXT("ItemsContainer")));
	

	if(!ItemsContainer)
	{
		UE_LOG(LogTemp, Display, TEXT("No name"));
		return;
	}

	UE_LOG(LogTemp, Display, TEXT("Name found"));

	ItemsContainer->ClearChildren();

	for(const AItem* Item : Items)
	{
		UImage* ItemImage = NewObject<UImage>(ItemsContainer);
		if(Item->ItemThumbnail)
		{
			ItemImage->SetBrushFromTexture(Item->ItemThumbnail);
		}

		UHorizontalBoxSlot* ItemSlot = ItemsContainer->AddChildToHorizontalBox(ItemImage);
		ItemSlot->SetPadding(FMargin(5.0f));
	}
}
