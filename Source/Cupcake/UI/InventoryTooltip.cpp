// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryTooltip.h"

#include "InventoryItemSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Cupcake/Items/BaseItem.h"

void UInventoryTooltip::NativeConstruct()
{
	Super::NativeConstruct();

	UBaseItem* ItemBeingHovered = InventorySlotBeingHovered->GetItemReference();

	switch (ItemBeingHovered->ItemType)
	{
	case EItemType::Wood:
		ItemType->SetText(FText::FromString("Consumable"));
		UsageText->SetVisibility(ESlateVisibility::Visible);
		break;
	case EItemType::Stone:
		break;
	case EItemType::Key:
		break;
	case EItemType::Quest:
		break;
	case EItemType::Misc:
		ItemType->SetText(FText::FromString("Consumable"));
		UsageText->SetVisibility(ESlateVisibility::Collapsed);
		break;
	default:
			;
	}

	ItemName->SetText(ItemBeingHovered->TextData.Name);
	UsageText->SetText(ItemBeingHovered->TextData.UsageText);
	ItemDescription->SetText(ItemBeingHovered->TextData.Description);

	if(ItemBeingHovered->NumericData.bIsStackable)
	{
		const FString StackInfo =
			{"Max stack size: "+ FString::FromInt(ItemBeingHovered->NumericData.MaxStackSize)};

		MaxStackSizeText->SetText(FText::FromString(StackInfo));
	}
	else
	{
		MaxStackSizeText->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UInventoryTooltip::UpdateTooltip(UBaseItem* Item)
{
	if (Item)
	{
		ItemName->SetText(Item->TextData.Name);
		UsageText->SetText(Item->TextData.UsageText);
		ItemDescription->SetText(Item->TextData.Description);

		switch (Item->ItemType)
		{
		case EItemType::Wood:
			ItemType->SetText(FText::FromString("Wood"));
			UsageText->SetVisibility(ESlateVisibility::Visible);
			break;
			// Add other cases as necessary
			default:;
		}

		if(Item->NumericData.bIsStackable)
		{
			MaxStackSizeText->SetText(FText::FromString(FString::Printf(TEXT("Max stack size: %d"), Item->NumericData.MaxStackSize)));
			MaxStackSizeText->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			MaxStackSizeText->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

