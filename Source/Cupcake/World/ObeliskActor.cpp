// Fill out your copyright notice in the Description page of Project Settings.


#include "ObeliskActor.h"

#include "Cupcake/Items/BaseItem.h"
#include "Cupcake/Items/Data/ItemDataStructs.h"
#include "Cupcake/PlayerSystem/NewInventoryComponent.h"

// Sets default values
AObeliskActor::AObeliskActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");

	SetRootComponent(Mesh);
	
}

// Called when the game starts or when spawned
void AObeliskActor::BeginPlay()
{
	Super::BeginPlay();

	InteractableData = InstanceInteractableData;
	InitializeObeliskItem(UBaseItem::StaticClass(), ItemQuantity);

	UE_LOG(LogTemp, Warning, TEXT("InventoryReference: %p"), InventoryReference);
	UE_LOG(LogTemp, Warning, TEXT("ItemReference: %p"), ItemReference);
	
}

void AObeliskActor::InitializeObeliskItem(const TSubclassOf<UBaseItem> BaseClass, const int32 InQuantity)
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

// Called every frame
void AObeliskActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AObeliskActor::BeginFocus()
{
	if(Mesh)
	{
		Mesh->SetRenderCustomDepth(true);
	}
}

void AObeliskActor::EndFocus()
{
	if(Mesh)
	{
		Mesh->SetRenderCustomDepth(false);
	}
}

void AObeliskActor::BeginInteract()
{
	IInteractionInterface::BeginInteract();
}

void AObeliskActor::EndInteract()
{
	IInteractionInterface::EndInteract();
}

void AObeliskActor::Interact(ACupcakeCharacter* PlayerCharacter)
{
	InventoryReference = PlayerCharacter->GetInventory();
	if(ItemReference)
	{
		UBaseItem* ItemToDonate = InventoryReference->FindMatchingItem(ItemReference);
		UE_LOG(LogTemp, Warning, TEXT("Test"));
		PlayerCharacter->RemoveItemFromInventory(ItemToDonate, ItemReference->Quantity);
		NumberOfItemsDonated++;
		InventoryReference->OnInventoryUpdated.Broadcast();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemRef was somehow null"));
	}

	FString InventoryDetails;
	for (const TObjectPtr<UBaseItem>& Item : InventoryReference->GetInventoryContents())
	{
		InventoryDetails += Item->ToString() + TEXT(", ");
	}

	UE_LOG(LogTemp, Warning, TEXT("InventoryContents: %s"), *InventoryDetails);
}


