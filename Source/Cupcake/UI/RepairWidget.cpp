// Fill out your copyright notice in the Description page of Project Settings.


#include "RepairWidget.h"

#include "Components/TextBlock.h"


void URepairWidget::IncreaseWoodQuantity(int NewWoodQuantity)
{
	if (WoodQuantity)
	{
		WoodQuantity->SetText(FText::AsNumber(NewWoodQuantity));
	}

}

void URepairWidget::IncreaseStoneQuantity(int NewStoneQuantity)
{
	if (StoneQuantity)
	{
		StoneQuantity->SetText(FText::AsNumber(NewStoneQuantity));
	}
}

void URepairWidget::IncreaseIronQuantity(int NewIronQuantity)
{
	if (IronQuantity)
	{
		IronQuantity->SetText(FText::AsNumber(NewIronQuantity));
	}
}
