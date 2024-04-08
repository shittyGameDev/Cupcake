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
		if(Item->bIsStackable)
		{
			AItem* ExistingItem = nullptr;
			for(AItem* InventoryItem : InventoryItems)
			{
				if (InventoryItem->ItemTypeId == Item->ItemTypeId)
				{
					ExistingItem = InventoryItem;
					break;
				}
			}

			if(ExistingItem)
			{
				ExistingItem->Quantity += Item->Quantity;
				UE_LOG(LogTemp, Warning, TEXT("Stacked item of type: %d, new quantity: %d"), Item->ItemTypeId, ExistingItem->Quantity);
				Item->Destroy();
			}
			else
			{
				InventoryItems.Add(Item);
				UE_LOG(LogTemp, Warning, TEXT("Added new stackable item of type: %d, quantity: %d"), Item->ItemTypeId, Item->Quantity);
			}
		}
		else
		{
			InventoryItems.Add(Item);
			UE_LOG(LogTemp, Warning, TEXT("Added non-stackable item: %s, quantity: %d"), *Item->ItemDescription, Item->Quantity);
		}
		Item->OnInteract();
	}
	
}

void UInventoryComponent::RemoveItem(AItem* Item)
{
	//
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

