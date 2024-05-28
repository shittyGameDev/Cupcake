#include "Pickup.h"

#include "Components/TimelineComponent.h"
#include "Cupcake/Items/BaseItem.h"
#include "Cupcake/PlayerSystem/CupcakeCharacter.h"
#include "Cupcake/PlayerSystem/NewInventoryComponent.h"

// Sets default values
APickup::APickup()
{
	PrimaryActorTick.bCanEverTick = true;

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
	PickupMesh->SetSimulatePhysics(true);
	SetRootComponent(PickupMesh);


	Timeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("PickupTimeline"));
	if (Timeline)
	{
		Timeline->SetAutoActivate(false);
	}

	bIsPickupable = false;

}



void APickup::BeginPlay()
{
	Super::BeginPlay();

	InitializePickup(UBaseItem::StaticClass(), ItemQuantity);
	GetWorld()->GetTimerManager().SetTimer(ValidationTimerHandle, this, &APickup::ValidateActors, 1.0f, true);
}

void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APickup::InitializePickup(const TSubclassOf<UBaseItem> BaseClass, const int32 InQuantity)
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

		PickupMesh->SetStaticMesh(ItemData->AssetData.Mesh);

		UpdateInteractableData();
	}
}

void APickup::UpdateInteractableData()
{
	InstanceInteractableData.InteractableType = EInteractableType::Pickup;
	InstanceInteractableData.Action = ItemReference->TextData.InteractionText;
	InstanceInteractableData.Name = ItemReference->TextData.Name;
	InstanceInteractableData.Quantity = ItemReference->Quantity;

	InteractableData = InstanceInteractableData;
}

void APickup::InitializeDrop(UBaseItem* ItemToDrop, const int32 InQuantity)
{
	ItemReference = ItemToDrop;
	InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);
	PickupMesh->SetStaticMesh(ItemToDrop->AssetData.Mesh);
	UpdateInteractableData();
}

void APickup::BeginFocus()
{
	if(PickupMesh)
	{
		PickupMesh->SetRenderCustomDepth(true);
	}
}

void APickup::EndFocus()
{
	if(PickupMesh)
	{
		PickupMesh->SetRenderCustomDepth(false);
	}
}

void APickup::Interact(ACupcakeCharacter* PlayerCharacter)
{
	if(PlayerCharacter && bIsPickupable)
	{
		TakePickup(PlayerCharacter);
	}
}

void APickup::TakePickup(const ACupcakeCharacter* Taker)
{
	if(!IsPendingKillPending())
	{
		if(ItemReference)
		{
			if(UNewInventoryComponent* PlayerInventory = Taker->GetInventory())
			{
				const FItemAddResult AddResult = PlayerInventory->HandleAddItem(ItemReference);

				switch (AddResult.OperationResult)
				{
				case EItemAddResult::Iar_NoItemAdded:
					break;
				case EItemAddResult::Iar_PartialAmountItemAdded:
					UE_LOG(LogTemp, Warning, TEXT("PartialAmountAdded"));
					UpdateInteractableData();
					Taker->UpdateInteractionWidget();
					break;
				case EItemAddResult::Iar_AllItemAdded:
					Destroy();
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

void APickup::ValidateActors()
{
	for (int32 i = ActorList.Num() - 1; i >= 0; --i)
	{
		if (!IsValid(ActorList[i]))
		{
			ActorList.RemoveAt(i);
		}
	}

	if(ActorList.IsEmpty())
	{
		bIsPickupable = true;
		DeactivateBarrier();
	}

}

