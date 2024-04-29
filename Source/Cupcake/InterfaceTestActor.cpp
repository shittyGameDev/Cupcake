// Fill out your copyright notice in the Description page of Project Settings.


#include "InterfaceTestActor.h"
#include "Cupcake/Items/BaseItem.h"
#include "PlayerSystem/CupcakeCharacter.h"
#include "PlayerSystem/NewInventoryComponent.h"

// Sets default values
AInterfaceTestActor::AInterfaceTestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");

	SetRootComponent(Mesh);
	

}


// Called when the game starts or when spawned
void AInterfaceTestActor::BeginPlay()
{
	Super::BeginPlay();

	InteractableData = InstanceInteractableData;

	InitializeCraftedItem(UBaseItem::StaticClass(), ItemQuantity);
}

void AInterfaceTestActor::InitializeCraftedItem(const TSubclassOf<UBaseItem> BaseClass, const int32 InQuantity)
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
void AInterfaceTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInterfaceTestActor::BeginFocus()
{
	if(Mesh)
	{
		Mesh->SetRenderCustomDepth(true);
	}
}

void AInterfaceTestActor::EndFocus()
{
	if(Mesh)
	{
		Mesh->SetRenderCustomDepth(false);
	}
}

void AInterfaceTestActor::Interact(ACupcakeCharacter* PlayerCharacter)
{
	if(!IsPendingKillPending())
	{
		if(ItemReference)
		{
			if(UNewInventoryComponent* PlayerInventory = PlayerCharacter->GetInventory())
			{
				const FItemAddResult AddResult = PlayerInventory->HandleAddItem(ItemReference);

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

				UE_LOG(LogTemp, Warning, TEXT("%s"), *AddResult.ResultMessage.ToString());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Player inventory component is null!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Puckip internal item reference was somehow null!"));
		}
	}
}


