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
	UE_LOG(LogTemp, Warning, TEXT("AddItem, before null check. Item ptr %s"), *Item->ItemName);
	if(Item != nullptr)
	{
		// Tries to find an existing item in the inventory with the same type ID
		AItem* ExistingItem = FindItemByName(Item->ItemName);

		// If the item is stackable and an item of the same type already exists in the inventory
		if(Item->bIsStackable && ExistingItem)
		{
			// Add the quantity of the new item to the existing items quantity
			ExistingItem->Quantity += Item->Quantity;
			UE_LOG(LogTemp, Warning, TEXT("Stacked item of type: %s, new quantity: %d"), *Item->ItemName, ExistingItem->Quantity);
		}
		else
		{
			// If no existing item is found or the item is not stackable add it as a new item to the inventory.
			InventoryItems.Add(Item);
			UE_LOG(LogTemp, Warning, TEXT("Added item: %s, quantity: %d"), *Item->ItemDescription, Item->Quantity);
		}
		// Atm kind of useless to call this as it is the same as "Item->Destroy()".
		UE_LOG(LogTemp, Warning, TEXT("Added item: %p"), ExistingItem);
		Item->Destroy();
		HotbarWidget->UpdateHotbar(InventoryItems);
	}
}

//Temporary function
void UInventoryComponent::RemoveItem(AItem* Item)
{
	if(Item != nullptr)
	{
		// Tries to find an existing item in the inventory with the same type ID
		AItem* ExistingItem = FindItemByName(Item->ItemName);
		
		// If the item is stackable, exists, and has more than one quantity
		if(Item->bIsStackable && ExistingItem && ExistingItem->Quantity > 1)
		{
			// Decrease the quantity of the existing item
			ExistingItem->Quantity--;
			UE_LOG(LogTemp, Warning, TEXT("Decreased quantity of item type: %s, new quantity: %d"), *ExistingItem->ItemName, ExistingItem->Quantity);
		}
		else
		{
			// If the item is not stackable or only one quantity exists, remove the item from the inventory
			InventoryItems.Remove(ExistingItem ? ExistingItem : Item);
			UE_LOG(LogTemp, Warning, TEXT("Removed item: %s"), *Item->ItemName);
		}
		HotbarWidget->UpdateHotbar(InventoryItems);
	}
}

AItem* UInventoryComponent::FindItemByName(FString ItemName)
{
	UE_LOG(LogTemp, Warning, TEXT("InvItems: %p"), &InventoryItems);
	for(AItem* InventoryItem : InventoryItems)
	{
		UE_LOG(LogTemp, Warning, TEXT("Item ptr: %p"), InventoryItem);
		UE_LOG(LogTemp, Warning, TEXT("InvItems Size: %d"), InventoryItems.Num());
		if(InventoryItem && InventoryItem->ItemName.ToLower().Equals(ItemName.ToLower()))
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

	UE_LOG(LogTemp, Warning, TEXT("Created new INV COMP"));

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn)
	{
		// Hämta spelarkontrollern från ägarpawnen
		APlayerController* PlayerController = Cast<APlayerController>(OwnerPawn->GetController());
		if (PlayerController)
		{
			// Skapa widgeten med hjälp av spelarkontrollern
			/*
			InventoryHUD = CreateWidget(PlayerController, InventoryHUDClass);
			if (InventoryHUD != nullptr)
			{
				InventoryHUD->AddToViewport();
			}
			*/

			HotbarWidget = CreateWidget<UHotbarWidget>(PlayerController, HotbarWidgetClass);
			if(HotbarWidget != nullptr)
			{
				HotbarWidget->AddToViewport();
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

