// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftingBenchActor.h"
#include "Cupcake/Items/BaseItem.h"
#include "Cupcake/PlayerSystem/NewInventoryComponent.h"


// Sets default values
ACraftingBenchActor::ACraftingBenchActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");

	SetRootComponent(Mesh);
	

}


// Called when the game starts or when spawned
void ACraftingBenchActor::BeginPlay()
{
	Super::BeginPlay();

	InteractableData = InstanceInteractableData;

	InitializeCraftedItem(UBaseItem::StaticClass(), ItemQuantity);
	InitializeCraftedItemCost(UBaseItem::StaticClass(), CraftedAmount);
}

void ACraftingBenchActor::InitializeCraftedItem(const TSubclassOf<UBaseItem> BaseClass, const int32 InQuantity)
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

void ACraftingBenchActor::InitializeCraftedItemCost(const TSubclassOf<UBaseItem> BaseClass, const int32 InQuantity)
{
	if(ItemDataTable && !RecipeItemID.IsNone())
	{
		const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(RecipeItemID, RecipeItemID.ToString());

		RecipeItem = NewObject<UBaseItem>(this, BaseClass);

		RecipeItem->ID = ItemData->ID;
		RecipeItem->ItemType = ItemData->ItemType;
		RecipeItem->TextData = ItemData->TextData;
		RecipeItem->NumericData = ItemData->NumericData;
		RecipeItem->AssetData = ItemData->AssetData;

		InQuantity <= 0 ? RecipeItem->SetQuantity(1) : RecipeItem->SetQuantity(InQuantity);
	}
}


// Called every frame
void ACraftingBenchActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACraftingBenchActor::BeginFocus()
{
	if(Mesh)
	{
		Mesh->SetRenderCustomDepth(true);
	}
}

void ACraftingBenchActor::EndFocus()
{
	if(Mesh)
	{
		Mesh->SetRenderCustomDepth(false);
	}
}

void ACraftingBenchActor::Interact(ACupcakeCharacter* PlayerCharacter)
{
	if(IsPendingKillPending())
		return;

	if(!IsValidItemAndRecipe())
	{
		UE_LOG(LogTemp, Warning, TEXT("Puckip internal item reference was somehow null!"));
		return;
	}

	UNewInventoryComponent* PlayerInventory = PlayerCharacter->GetInventory();
	if (!PlayerInventory)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player inventory component is null!"));
		return;
	}

	if (!PlayerInventory->HasItemByID(RecipeItemID))
		return;

	HandleItemRemovalAndAddition(PlayerCharacter, PlayerInventory);
}

bool ACraftingBenchActor::IsValidItemAndRecipe() const
{
	return ItemReference && RecipeItem;
}

void ACraftingBenchActor::HandleItemRemovalAndAddition(ACupcakeCharacter* PlayerCharacter, UNewInventoryComponent* PlayerInventory)
{
	UBaseItem* ItemToRemove = PlayerInventory->FindMatchingItem(RecipeItem);
	if(ItemToRemove)
	{
		UE_LOG(LogTemp, Warning, TEXT("Test"));
		PlayerCharacter->RemoveItemFromInventory(ItemToRemove, RecipeItem->Quantity);
		PlayerInventory->OnInventoryUpdated.Broadcast();
	}

	FItemAddResult AddResult = PlayerInventory->HandleAddItem(ItemReference);
	HandleAddResult(AddResult, PlayerCharacter);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *AddResult.ResultMessage.ToString());
}

void ACraftingBenchActor::HandleAddResult(const FItemAddResult& AddResult, ACupcakeCharacter* PlayerCharacter) const
{
	switch (AddResult.OperationResult)
	{
	case EItemAddResult::Iar_NoItemAdded:
		break;
	case EItemAddResult::Iar_PartialAmountItemAdded:
		PlayerCharacter->UpdateInteractionWidget();
		break;
	case EItemAddResult::Iar_AllItemAdded:
		break;
	}
}



