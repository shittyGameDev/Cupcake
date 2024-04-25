// Fill out your copyright notice in the Description page of Project Settings.


#include "TheMapHandler.h"

#include "Components/Image.h"

void UTheMapHandler::NativeConstruct()
{
	Super::NativeConstruct();
	// Lägg till dessa loggmeddelanden för att verifiera att bilderna läggs till korrekt
	Images.Add("Image1", Image1); UE_LOG(LogTemp, Warning, TEXT("Added Image1"));
	Images.Add("Image2", Image2); UE_LOG(LogTemp, Warning, TEXT("Added Image2"));
	Images.Add("Image3", Image3); UE_LOG(LogTemp, Warning, TEXT("Added Image3"));
	Images.Add("Image4", Image4); UE_LOG(LogTemp, Warning, TEXT("Added Image4"));
	UE_LOG(LogTemp, Warning, TEXT("Printing Images Map:"));
	for (const auto& Pair : Images)
	{
		FString Key = Pair.Key.ToString();
		UImage* Image = Pair.Value;
		if (Image)
		{
			// Skriv ut taggen och namnet på UImage-objektet, om det finns ett mer specifikt namn kan du byta ut GetName() mot relevant funktion
			UE_LOG(LogTemp, Warning, TEXT("Tag: %s, Image Name: %s"), *Key, *Image->GetName());
		}
		else
		{
			// Meddela om bildpekaren är null
			UE_LOG(LogTemp, Warning, TEXT("Tag: %s, Image is nullptr"), *Key);
		}
	}
}

UImage* UTheMapHandler::GetImageByTag(const FName& Tag)
{
	Images.Add("Image1", Image1); UE_LOG(LogTemp, Warning, TEXT("Added Image1"));
	Images.Add("Image2", Image2); UE_LOG(LogTemp, Warning, TEXT("Added Image2"));
	Images.Add("Image3", Image3); UE_LOG(LogTemp, Warning, TEXT("Added Image3"));
	Images.Add("Image4", Image4); UE_LOG(LogTemp, Warning, TEXT("Added Image4"));
	UE_LOG(LogTemp, Warning, TEXT("Searching for Image with Tag: %s"), *Tag.ToString());
	UImage** FoundImage = Images.Find(Tag);
	UE_LOG(LogTemp, Warning, TEXT("Printing Images Map:"));

	if (FoundImage)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found Image with Tag: %s"), *Tag.ToString());
		return *FoundImage;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Image found with Tag: %s"), *Tag.ToString());
		return nullptr;
	}
}

void UTheMapHandler::SetImageOpacity(const FName& ImageTag, float NewOpacity)
{
	UImage* TargetImage = GetImageByTag(ImageTag);
	if (TargetImage)
	{
		TargetImage->SetOpacity(NewOpacity);
	}
}

