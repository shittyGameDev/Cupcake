// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseItem.h"

#include "Cupcake/PlayerSystem/NewInventoryComponent.h"

UBaseItem::UBaseItem() : bIsCopy(false), bIsPickup(false)
{
}

void UBaseItem::ResetItemFlags()
{
	bIsCopy = false;
	bIsPickup = false;
}

UBaseItem* UBaseItem::CreateItemCopy() const
{
	UBaseItem* ItemCopy = NewObject<UBaseItem>(StaticClass());

	ItemCopy->ID = this->ID;
	ItemCopy->Quantity = this->Quantity;
	ItemCopy->ItemType = this->ItemType;
	ItemCopy->TextData = this->TextData;
	ItemCopy->NumericData = this->NumericData;
	ItemCopy->AssetData = this->AssetData;
	ItemCopy->bIsCopy = true;

	return ItemCopy;
}

void UBaseItem::SetQuantity(const int32 NewQuantity)
{
	if(NewQuantity != Quantity)
	{
		Quantity = FMath::Clamp(NewQuantity, 0, NumericData.bIsStackable ? NumericData.MaxStackSize : 1);

		if(OwningInventory)
		{
			if(Quantity <= 0)
			{
				OwningInventory->RemoveSingleInstanceOfItem(this);
			}
		}
	}
}

void UBaseItem::Use(ACupcakeCharacter* Character)
{
}

FString UBaseItem::ToString() const
{
	// Customize this to include whatever item details are important
	return FString::Printf(TEXT("ItemName: %s, ItemID: %d"), *ID.ToString(), Quantity);
}

