// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

#include "Blueprint/UserWidget.h"

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
		// Tries to find an existing item in the inventory with the same type ID
		AItem* ExistingItem = FindItemById(Item->ItemTypeId);

		// If the item is stackable and an item of the same type already exists in the inventory
		if(Item->bIsStackable && ExistingItem)
		{
			// Add the quantity of the new item to the existing items quantity
			ExistingItem->Quantity += Item->Quantity;
			UE_LOG(LogTemp, Warning, TEXT("Stacked item of type: %d, new quantity: %d"), Item->ItemTypeId, ExistingItem->Quantity);
			// Atm kind of useless to call this as it is the same as "Item->Destroy()".
			Item->Destroy();
		}
		else
		{
			// If no existing item is found or the item is not stackable add it as a new item to the inventory.
			InventoryItems.Add(Item);
			UE_LOG(LogTemp, Warning, TEXT("Added item: %s, quantity: %d"), *Item->ItemDescription, Item->Quantity);
		}
		// Atm kind of useless to call this as it is the same as "Item->Destroy()".
		Item->Destroy();
	}
}

//Temporary function
void UInventoryComponent::RemoveItem(AItem* Item)
{
	if(Item != nullptr)
	{
		// Tries to find an existing item in the inventory with the same type ID
		AItem* ExistingItem = FindItemById(Item->ItemTypeId);
		
		// If the item is stackable, exists, and has more than one quantity
		if(Item->bIsStackable && ExistingItem && ExistingItem->Quantity > 1)
		{
			// Decrease the quantity of the existing item
			ExistingItem->Quantity--;
			UE_LOG(LogTemp, Warning, TEXT("Decreased quantity of item type: %d, new quantity: %d"), ExistingItem->ItemTypeId, ExistingItem->Quantity);
		}
		else
		{
			// If the item is not stackable or only one quantity exists, remove the item from the inventory
			InventoryItems.Remove(ExistingItem ? ExistingItem : Item);
			/*
			if(ExistingItem)
			{
				ExistingItem->Destroy();
			}
			else
			{
				Item->Destroy();
			}
			*/
			UE_LOG(LogTemp, Warning, TEXT("Removed item: %d"), Item->ItemTypeId);
		}
	}
}

AItem* UInventoryComponent::FindItemById(int32 ItemTypeId)
{
	for(AItem* InventoryItem : InventoryItems)
	{
		if(InventoryItem && InventoryItem->ItemTypeId == ItemTypeId)
		{
			return InventoryItem;
		}
	}
	return nullptr;
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn)
	{
		// Hämta spelarkontrollern från ägarpawnen
		APlayerController* PlayerController = Cast<APlayerController>(OwnerPawn->GetController());
		if (PlayerController)
		{
			// Skapa widgeten med hjälp av spelarkontrollern
			InventoryHUD = CreateWidget(PlayerController, InventoryHUDClass);
			if (InventoryHUD != nullptr)
			{
				InventoryHUD->AddToViewport();
			}
		}
	}
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
	// ...
}

