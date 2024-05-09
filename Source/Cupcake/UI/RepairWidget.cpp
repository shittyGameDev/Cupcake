// Fill out your copyright notice in the Description page of Project Settings.


#include "RepairWidget.h"

#include "Components/TextBlock.h"


void URepairWidget::IncreaseWoodQuantity(int NewWoodQuantity)
{
	if (WoodQuantity && WoodImage && WoodAnim)
	{
		WoodQuantity->SetText(FText::AsNumber(NewWoodQuantity));
		PlayAnimation(WoodAnim);
	}

}

void URepairWidget::IncreaseStoneQuantity(int NewStoneQuantity)
{
	if (StoneQuantity && StoneImage && StoneAnim)
	{
		StoneQuantity->SetText(FText::AsNumber(NewStoneQuantity));
		PlayAnimation(StoneAnim);
	}
}

void URepairWidget::IncreaseIronQuantity(int NewIronQuantity)
{
	if (IronQuantity && IronImage && IronAnim)
	{
		IronQuantity->SetText(FText::AsNumber(NewIronQuantity));
		PlayAnimation(IronAnim);
	}
}
