// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseItem.h"

UBaseItem::UBaseItem()
{
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

	return ItemCopy;
}

void UBaseItem::SetQuantity(const int32 NewQuantity)
{
	if(NewQuantity != Quantity)
	{
		Quantity = FMath::Clamp(NewQuantity, 0, NumericData.bIsStackable ? NumericData.MaxStackSize : 1);

		//if(OwningInventory)
		//{
		//	if(Quantity <= 0)
		//	{
		//		OwningInventory->RemoveItem(this);
		//	}
		//}
	}
}

void UBaseItem::Use(ACupcakeCharacter* Character)
{
}
