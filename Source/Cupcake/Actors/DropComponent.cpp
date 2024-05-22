// Fill out your copyright notice in the Description page of Project Settings.


#include "DropComponent.h"

#include "Cupcake/Items/BaseItem.h"
#include "Cupcake/Items/Data/ItemDataStructs.h"
#include "Cupcake/PlayerSystem/NewInventoryComponent.h"

// Sets default values for this component's properties
UDropComponent::UDropComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UDropComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeDrop(UBaseItem::StaticClass(), ItemQuantity);
	
}

void UDropComponent::DropItem(ACupcakeCharacter* PlayerCharacter) const
{
	if(ItemReference)
	{
		UNewInventoryComponent* PlayerInventory = PlayerCharacter->GetInventory();
		PlayerInventory->HandleAddItem(ItemReference);
	}
}

void UDropComponent::InitializeDrop(const TSubclassOf<UBaseItem> BaseClass, const int32 InQuantity)
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



