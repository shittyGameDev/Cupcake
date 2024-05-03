#include "Pickup.h"

#include "Components/TimelineComponent.h"
#include "Cupcake/Items/BaseItem.h"
#include "Cupcake/PlayerSystem/CupcakeCharacter.h"
#include "Cupcake/PlayerSystem/NewInventoryComponent.h"
#include "Math/UnitConversion.h"

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

}

void APickup::BeginPlay()
{
	Super::BeginPlay();

	InitializePickup(UBaseItem::StaticClass(), ItemQuantity);
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

void APickup::StartScaling(UCurveFloat* ScaleCurve)
{
	if (ScaleCurve)
	{
		// Stänger av fysiken just nu för att senare felsöka.
		//TODO: Felsök varför en massa errors uppstår om fysik är på.
		PickupMesh->SetSimulatePhysics(false);

		FOnTimelineFloat ProgressFunction;
		ProgressFunction.BindUFunction(this, FName("HandleScaling"));
		Timeline->AddInterpFloat(ScaleCurve, ProgressFunction);

		FOnTimelineEvent TimelineFinishedFunction;
		TimelineFinishedFunction.BindUFunction(this, FName("FinishScaling"));
		Timeline->SetTimelineFinishedFunc(TimelineFinishedFunction);

		UE_LOG(LogTemp, Warning, TEXT("Curve keys count: %d"), ScaleCurve->FloatCurve.Keys.Num());
		Timeline->PlayFromStart();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ScaleCurve is NULL"));
	}
}

void APickup::HandleScaling(float Value)
{
	// Förändrar skalan baserat på float curve.
	SetActorScale3D(FVector(Value, Value, Value));
}

void APickup::FinishScaling() const
{
	// Just nu testar vi att inte slå på fysiken, saker flyger helt galet annars.
	//PickupMesh->SetSimulatePhysics(true);

	// Loggar bara för o se när scalingen slutar.
	UE_LOG(LogTemp, Warning, TEXT("Scaling completed for %s"), *GetName());
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
	if(PlayerCharacter)
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

