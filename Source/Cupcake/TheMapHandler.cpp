// Fill out your copyright notice in the Description page of Project Settings.

#include "TheMapHandler.h"
#include "Components/Image.h"

UTheMapHandler* UTheMapHandler::Instance = nullptr;

UTheMapHandler::UTheMapHandler(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer) 
{

}

UTheMapHandler* UTheMapHandler::GetInstance()
{
	if (!Instance)
	{
		Instance = NewObject<UTheMapHandler>(); // Create a new instance of the widget
		Instance->AddToRoot(); // Prevent garbage collection
	}
	return Instance;
}

void UTheMapHandler::NativeConstruct()
{
	Super::NativeConstruct();
	// Skriv ut värdet av bild-referenser
	UE_LOG(LogTemp, Warning, TEXT("Image1: %p"), Image1);
	UE_LOG(LogTemp, Warning, TEXT("Image2: %p"), Image2);
	UE_LOG(LogTemp, Warning, TEXT("Image3: %p"), Image3);
	UE_LOG(LogTemp, Warning, TEXT("Image4: %p"), Image4);
	// Skriv ut värdet igen efter försöket att hitta widgetarna
	UE_LOG(LogTemp, Warning, TEXT("Image1 efter försök att hitta: %p"), Image1);
	UE_LOG(LogTemp, Warning, TEXT("Image2 efter försök att hitta: %p"), Image2);
	UE_LOG(LogTemp, Warning, TEXT("Image3 efter försök att hitta: %p"), Image3);
	UE_LOG(LogTemp, Warning, TEXT("Image4 efter försök att hitta: %p"), Image4);
	if (Image3)
	{
		UE_LOG(LogTemp, Warning, TEXT("Image3 hittades och inställdes korrekt!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Image3 hittades inte eller kunde inte inställas korrekt."));
	}
/*
	// Add images to map and log
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
			UE_LOG(LogTemp, Warning, TEXT("Tag: %s, Image Name: %s"), *Key, *Image->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Tag: %s, Image is nullptr"), *Key);
		}
	}*/
}

UImage* UTheMapHandler::GetImageByTag(const FName& Tag)
{
	if (Images.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Images map is empty!"));
	}
	UImage** FoundImage = Images.Find(Tag);
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

void UTheMapHandler::printImage1()
{
	UE_LOG(LogTemp, Warning, TEXT("Image1 efter försök att hitta: %p"), Image1);
}
