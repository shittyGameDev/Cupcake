// Fill out your copyright notice in the Description page of Project Settings.

#include "TheMapHandler.h"
#include "Components/Image.h"


void UTheMapHandler::HideImage(int32 ImageIndex)
{
	switch (ImageIndex)
	{
	case 1:
		if (Image1)
			Image1->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case 2:
		if (Image2)
			Image2->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case 3:
		if (Image3)
			Image3->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case 4:
		if (Image4)
			Image4->SetVisibility(ESlateVisibility::Collapsed);
			Image4->SetVisibility(ESlateVisibility::Collapsed);
		break;
	default:
		break;
	}
}







