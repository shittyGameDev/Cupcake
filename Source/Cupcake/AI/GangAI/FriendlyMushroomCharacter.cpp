// Fill out your copyright notice in the Description page of Project Settings.


#include "FriendlyMushroomCharacter.h"

#include "AIController.h"
#include "Cupcake/Items/BaseItem.h"
#include "Cupcake/Items/Data/ItemDataStructs.h"
#include "Cupcake/PlayerSystem/NewInventoryComponent.h"

AFriendlyMushroomCharacter::AFriendlyMushroomCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AFriendlyMushroomCharacter::InitMushroomItem(const TSubclassOf<UBaseItem> BaseClass, const int32 InQuantity)
{
	if(ItemDataTable && !DesiredItemID.IsNone())
	{
		const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString());

		ItemReference = NewObject<UBaseItem>(this, BaseClass);

		ItemReference->ID = ItemData->ID;
		ItemReference->ItemType = ItemData->ItemType;
		ItemReference->TextData = ItemData->TextData;
		ItemReference->NumericData = ItemData->NumericData;
		ItemReference->AssetData = ItemData->AssetData;

		InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);
	}
}

void AFriendlyMushroomCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitMushroomItem(UBaseItem::StaticClass(), ItemQuantity);
}

void AFriendlyMushroomCharacter::MoveTrigger()
{
	if (MoveLocations.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("No move locations"));
		return;
	}

	MoveToNextLocation();
}

void AFriendlyMushroomCharacter::MoveToNextLocation()
{
	FVector MoveLocation = MoveLocations.Pop()->GetActorLocation();
	MoveToLocation(MoveLocation);	
}

void AFriendlyMushroomCharacter::MoveToLocation(const FVector& NewLocation)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		AIController->MoveToLocation(NewLocation);
	}
}

// Interactable
void AFriendlyMushroomCharacter::BeginFocus()
{
	IInteractionInterface::BeginFocus();
}
void AFriendlyMushroomCharacter::EndFocus()
{
	IInteractionInterface::EndFocus();
}
void AFriendlyMushroomCharacter::BeginInteract()
{
	IInteractionInterface::BeginInteract();
}
void AFriendlyMushroomCharacter::EndInteract()
{
	IInteractionInterface::EndInteract();
}

void AFriendlyMushroomCharacter::Interact(ACupcakeCharacter* PlayerCharacter)
{
	IInteractionInterface::Interact(PlayerCharacter);

	// Get item from player
	UBaseItem* ItemToTake = PlayerCharacter->GetInventory()->FindMatchingItem(ItemReference);
	if (ItemToTake)
	{
		RunRemoveComponent();
		
		PlayerCharacter->GetInventory()->RemoveAmountOfItem(ItemToTake, ItemReference->Quantity);

		if (MoveLocations.IsEmpty())
		{
			UE_LOG(LogTemp, Warning, TEXT("No move locations"));
			return;
		}
		
		MoveToNextLocation();
	}
}



