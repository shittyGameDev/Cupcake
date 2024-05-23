// Fill out your copyright notice in the Description page of Project Settings.


#include "NewInventoryComponent.h"

#include "Cupcake/Items/BaseItem.h"

// Sets default values for this component's properties
UNewInventoryComponent::UNewInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UNewInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

UBaseItem* UNewInventoryComponent::FindMatchingItem(UBaseItem* ItemIn) const
{
	if (ItemIn)
	{
		for (const TObjectPtr<UBaseItem>& Item : InventoryContents)
		{
			if (Item->ID == ItemIn->ID) // Ensure this compares the unique identifiers correctly
				{
				return Item.Get();
				}
		}
	}
	return nullptr;
}


UBaseItem* UNewInventoryComponent::FindNextItemByID(UBaseItem* ItemIn) const
{
	if(ItemIn)
	{
		if(const TArray<TObjectPtr<UBaseItem>>::ElementType* Result = InventoryContents.FindByKey(ItemIn))
		{
			return *Result;
		}
	}
	// Assuming UBaseItem has a ToString method that returns FString
	FString InventoryDetails;
	for (const TObjectPtr<UBaseItem>& Item : InventoryContents)
	{
		InventoryDetails += Item->ToString() + TEXT(", ");
	}

	UE_LOG(LogTemp, Warning, TEXT("InventoryContents: %s"), *InventoryDetails);
	return nullptr;
}

UBaseItem* UNewInventoryComponent::FindNextPartialStack(UBaseItem* ItemIn) const
{
	if (const TArray<TObjectPtr<UBaseItem>>::ElementType* Result =
		InventoryContents.FindByPredicate([&ItemIn](const UBaseItem* InventoryItem)
		{
			return InventoryItem->ID == ItemIn->ID && !InventoryItem->IsFullItemStack();
		}))
	{
		return *Result;
	}
	return nullptr;
}


int32 UNewInventoryComponent::CalculateNumberForFullStack(UBaseItem* StackableItem, int32 InitialRequestedAddAmount)
{
	const int32 AddAmountToMakeFullStack = StackableItem->NumericData.MaxStackSize - StackableItem->Quantity;

	return FMath::Min(InitialRequestedAddAmount, AddAmountToMakeFullStack);
}

void UNewInventoryComponent::RemoveSingleInstanceOfItem(UBaseItem* ItemToRemove)
{
	InventoryContents.RemoveSingle(ItemToRemove);
	//Delegate that UI listens to.
	UE_LOG(LogTemp, Warning, TEXT("Item was removed: %s"), *ItemToRemove->ID.ToString());

	OnInventoryUpdated.Broadcast();
}

int32 UNewInventoryComponent::RemoveAmountOfItem(UBaseItem* ItemIn, int32 DesiredAmountToRemove)
{
	const int32 ActualAmountToRemove = FMath::Min(DesiredAmountToRemove, ItemIn->Quantity);
	OnRemoveItem.Broadcast(ItemIn, ActualAmountToRemove);
	ItemIn->SetQuantity(ItemIn->Quantity - ActualAmountToRemove);


	OnInventoryUpdated.Broadcast();

	return ActualAmountToRemove;
}

void UNewInventoryComponent::SplitExistingStack(UBaseItem* ItemIn, const int32 AmountToSplit)
{
	// If one more thing will not overflow the inventory --> (Victor)
	if(!(InventoryContents.Num() + 1 <= InventorySlotsCapacity))
	{
		//We remove the amount to split and add it as a new entry to the inventory (Victor)
		RemoveAmountOfItem(ItemIn, AmountToSplit);
		AddNewItem(ItemIn, AmountToSplit);
	}
}

bool UNewInventoryComponent::HasItemByID(FName ID)
{
	for (auto Element : InventoryContents)
	{
		if (Element->ID.IsEqual(ID))
		{
			return true;
		}
	}
	return false;
}

FItemAddResult UNewInventoryComponent::HandleNonStackableItems(UBaseItem* InputItem, int32 RequestedAddAmount)
{
	//If adding 1 more item to the inventory would overflow slot capacity
	if(InventoryContents.Num() + 1 > InventorySlotsCapacity)
	{
		//Return added none
		return FItemAddResult::AddedNone(FText::Format(
			FText::FromString("Could not add {0} to the inventory. All inventory slots are full."),
			InputItem->TextData.Name));
	}

	AddNewItem(InputItem, RequestedAddAmount);
	// return added all result
	return FItemAddResult::AddedAll(RequestedAddAmount,FText::Format(
			FText::FromString("Successfully added {0} {1} to the inventory."),
			RequestedAddAmount,
			InputItem->TextData.Name));
}

int32 UNewInventoryComponent::HandleStackableItems(UBaseItem* InputItem, int32 RequestedAddAmount)
{
	if (RequestedAddAmount <= 0)
	{
		// Invalid item data
		return 0;
	}

	int32 AmountToDistribute = RequestedAddAmount;
	UBaseItem* ExistingItemStack = FindNextPartialStack(InputItem);

	// Distribute item stack over existing partial stacks
	while (ExistingItemStack != nullptr)
	{
		const int32 AmountToAdd = CalculateNumberForFullStack(ExistingItemStack, AmountToDistribute);

		if (AmountToAdd > 0)
		{
			ExistingItemStack->SetQuantity(ExistingItemStack->Quantity + AmountToAdd);
			AmountToDistribute -= AmountToAdd;

			if (AmountToDistribute == 0)
			{
				OnInventoryAdd.Broadcast();
				OnInventoryUpdated.Broadcast();
				return RequestedAddAmount;
			}
		}

		ExistingItemStack = FindNextPartialStack(InputItem);
	}

	// If there are still items left to distribute and there is an available slot
	if (AmountToDistribute > 0 && InventoryContents.Num() < InventorySlotsCapacity)
	{
		AddNewItem(InputItem, AmountToDistribute);
		OnInventoryAdd.Broadcast();
		OnInventoryUpdated.Broadcast();
		return RequestedAddAmount;
	}

	// If all partial stacks are processed and no new slots are available, but items still need to be distributed
	return RequestedAddAmount - AmountToDistribute;
}


FItemAddResult UNewInventoryComponent::HandleAddItem(UBaseItem* InputItem)
{
	UE_LOG(LogTemp, Warning, TEXT("HandleAddItem: %p"), InputItem);
	// Check for valid owner (Victor)
	if(GetOwner())
	{
		const int32 InitialRequestedAddAmount = InputItem->Quantity;

		// Handle non-stackable items (Victor)
		if(!InputItem->NumericData.bIsStackable)
		{
			return HandleNonStackableItems(InputItem, InitialRequestedAddAmount);
		}

		// Handle stackable items (Victor)
		const int32 StackableAmountAdded = HandleStackableItems(InputItem, InitialRequestedAddAmount);

		if(StackableAmountAdded == InitialRequestedAddAmount)
		{
			
			OnPickup.Broadcast(InputItem);
			return FItemAddResult::AddedAll(InitialRequestedAddAmount, FText::Format(
				FText::FromString("Successfully added {0} {1}(s) to the inventory."),
				InitialRequestedAddAmount,
				InputItem->TextData.Name));
		}

		if(StackableAmountAdded < InitialRequestedAddAmount && StackableAmountAdded > 0)
		{
			OnPickup.Broadcast(InputItem);
			OnInventoryAdd.Broadcast();
			OnInventoryUpdated.Broadcast();
			return FItemAddResult::AddedPartial(StackableAmountAdded, FText::Format(
				FText::FromString("Partial amount of {0} added to the inventory. Number added = {1}"),
				InputItem->TextData.Name,
				StackableAmountAdded));
			
		}

		if(StackableAmountAdded <= 0)
		{
			return FItemAddResult::AddedNone(FText::Format(
				FText::FromString("Couldn't add {0} to the inventory. Inventory is full"),
				InputItem->TextData.Name));
		}
	}

	check(false);
	return FItemAddResult::AddedNone(FText::FromString("TryAddItem fallthrough error. GetOwner() check somehow failed"));
}

FItemAddResult UNewInventoryComponent::HandleLoadItem(UBaseItem* InputItem)
{
	UE_LOG(LogTemp, Warning, TEXT("HandleAddItem: %p"), InputItem);
	// Check for valid owner (Victor)
	if(GetOwner())
	{
		const int32 InitialRequestedAddAmount = InputItem->Quantity;

		// Handle non-stackable items (Victor)
		if(!InputItem->NumericData.bIsStackable)
		{
			return HandleNonStackableItems(InputItem, InitialRequestedAddAmount);
		}

		// Handle stackable items (Victor)
		const int32 StackableAmountAdded = HandleStackableItems(InputItem, InitialRequestedAddAmount);

		if(StackableAmountAdded == InitialRequestedAddAmount)
		{
			
			return FItemAddResult::AddedAll(InitialRequestedAddAmount, FText::Format(
				FText::FromString("Successfully added {0} {1}(s) to the inventory."),
				InitialRequestedAddAmount,
				InputItem->TextData.Name));
		}

		if(StackableAmountAdded < InitialRequestedAddAmount && StackableAmountAdded > 0)
		{
			OnInventoryUpdated.Broadcast();
			return FItemAddResult::AddedPartial(StackableAmountAdded, FText::Format(
				FText::FromString("Partial amount of {0} added to the inventory. Number added = {1}"),
				InputItem->TextData.Name,
				StackableAmountAdded));
			
		}

		if(StackableAmountAdded <= 0)
		{
			return FItemAddResult::AddedNone(FText::Format(
				FText::FromString("Couldn't add {0} to the inventory. Inventory is full"),
				InputItem->TextData.Name));
		}
	}

	check(false);
	return FItemAddResult::AddedNone(FText::FromString("TryAddItem fallthrough error. GetOwner() check somehow failed"));
}

void UNewInventoryComponent::ClearInventory()
{
	InventoryContents.Empty();
}

void UNewInventoryComponent::AddNewItem(UBaseItem* Item, const int32 AmountToAdd)
{
	UBaseItem* NewItem;

	if(Item->ID.IsEqual("forestItem"))
	{
		OnKeyItemAdded.Broadcast();
	} else if (Item->ID.IsEqual("caveItem"))
	{
		OnKeyItemAdded.Broadcast();
	}
	else if (Item->ID.IsEqual("hillItem"))
	{
		OnKeyItemAdded.Broadcast();
	}

	if(Item->bIsCopy|| Item->bIsPickup)
	{
		//If the item is already a copy, or is a world pickup
		NewItem = Item;
		NewItem->ResetItemFlags();
	}
	else
	{
		// Used when splitting or dragging to/from another inventory
		NewItem = Item->CreateItemCopy();
	}

	NewItem->OwningInventory = this;
	NewItem->SetQuantity(AmountToAdd);

	OnPickup.Broadcast(NewItem);
	InventoryContents.Add(NewItem);
	OnInventoryAdd.Broadcast();
	OnInventoryUpdated.Broadcast();
}







