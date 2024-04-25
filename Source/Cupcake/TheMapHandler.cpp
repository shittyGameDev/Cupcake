// Fill out your copyright notice in the Description page of Project Settings.


#include "TheMapHandler.h"

#include "Components/Image.h"

void UTheMapHandler::NativeConstruct()
{
	Super::NativeConstruct();
	Images.Add("Image1", Image1);
	Images.Add("Image2", Image2);
	Images.Add("Image3", Image3);
	Images.Add("Image4", Image4);
}

UImage* UTheMapHandler::GetImageByTag(const FName& Tag)
{
	UImage** FoundImage = Images.Find(Tag);
	if (FoundImage)
	{
		return *FoundImage;
	}
	return nullptr;
}

void UTheMapHandler::SetImageOpacity(const FName& ImageTag, float NewOpacity)
{
	UImage* TargetImage = GetImageByTag(ImageTag);
	if (TargetImage)
	{
		TargetImage->SetOpacity(NewOpacity);
	}
}

