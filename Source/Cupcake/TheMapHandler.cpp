// Fill out your copyright notice in the Description page of Project Settings.

#include "TheMapHandler.h"
#include "Components/Image.h"



void UTheMapHandler::HideImage(int32 ImageIndex)
{
	switch (ImageIndex)
	{
	case 1:
		if (Image1)
		{
			Image1->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f)); // Full opacitet
		}else
		{
			UE_LOG(LogTemp, Warning, TEXT("Hittar inte image1"));
		}
		break;
	case 2:
		if (Image2)
		{
			Image2->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f)); // Full opacitet
		}
		break;
	case 3:
		if (Image3)
		{
			Image3->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f)); // Full opacitet
		}
		break;
	case 4:
		if (Image4)
		{
			Image4->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f)); // Full opacitet
		}
		break;
	default:
		break;
	}
}







