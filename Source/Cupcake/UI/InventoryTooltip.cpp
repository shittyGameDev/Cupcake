// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryTooltip.h"

#include "InventoryItemSlot.h"
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
	default: ;
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
