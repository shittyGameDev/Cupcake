// Fill out your copyright notice in the Description page of Project Settings.


#include "MapUI.h"
#include "Engine/TriggerBox.h"
#include "Components/Widget.h"

// Add the implementation of ShowImagesOnTrigger function
void UMapUI::ShowImagesOnTrigger(const FName& TriggerBoxName)

{
	// Iterate over Images array and set each image's visibility
	for (UImage* ImagePtr : Images)
	{
		if (ImagePtr)
		{
			// Set visibility to true to make the image visible
			ImagePtr->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

