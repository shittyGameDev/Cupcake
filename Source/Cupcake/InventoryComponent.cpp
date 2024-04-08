// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

//
void UInventoryComponent::AddItem(AItem* Item)
{
	if(Item != nullptr)
	{
		//If item is stackable we handle the logic to stack the items.
		if(Item->bIsStackable)
		{
			//Variable to hold existing items.
			AItem* ExistingItem = nullptr;
			//Goes through every Item in InventoryItems and checks ItemTypeId (specified for each item)
			for(AItem* InventoryItem : InventoryItems)
			{
				if (InventoryItem->ItemTypeId == Item->ItemTypeId)
				{
					ExistingItem = InventoryItem;
					break;
				}
			}

			//If item exists in the Inventory array we add to the quantity of existing item with the amount specified in the item blueprint
			//We can use ExistingItem to show the player the amount through UI.
			if(ExistingItem)
			{
				ExistingItem->Quantity += Item->Quantity;
				UE_LOG(LogTemp, Warning, TEXT("Stacked item of type: %d, new quantity: %d"), Item->ItemTypeId, ExistingItem->Quantity);
				Item->OnInteract();
			}
			else
			{
				//If the item does not exist in the array we add it as a new item instead.
				InventoryItems.Add(Item);
				UE_LOG(LogTemp, Warning, TEXT("Added new stackable item of type: %d, quantity: %d"), Item->ItemTypeId, Item->Quantity);
			}
		}
		else
		{
			//If item is not stackable we make sure to add it anyways but we do not need to handle the stacking of the item.
			InventoryItems.Add(Item);
			UE_LOG(LogTemp, Warning, TEXT("Added non-stackable item: %s, quantity: %d"), *Item->ItemDescription, Item->Quantity);
		}
		Item->OnInteract();
	}
	
}

//TODO: Add RemoveItem logic.
void UInventoryComponent::RemoveItem(AItem* Item)
{
	//Method should check the ItemTypeId and remove from list.

	//If the item is stackable maybe we should handle the amount being Removed.
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

