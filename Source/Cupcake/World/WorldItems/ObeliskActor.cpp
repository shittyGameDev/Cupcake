// Fill out your copyright notice in the Description page of Project Settings.


#include "ObeliskActor.h"

#include "NiagaraComponent.h"
#include "Components/TextBlock.h"
#include "Cupcake/Items/BaseItem.h"
#include "Cupcake/Items/Data/ItemDataStructs.h"
#include "Cupcake/UI/RepairWidget.h"
#include "Cupcake/PlayerSystem/NewInventoryComponent.h"
#include "Cupcake/UI/EndScreen.h"

// Sets default values
AObeliskActor::AObeliskActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");

	SetRootComponent(Mesh);

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("NiagaraComponent");
	NiagaraComponent->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void AObeliskActor::BeginPlay()
{
	Super::BeginPlay();

	InteractableData = InstanceInteractableData;
	InitializeObeliskItem(UBaseItem::StaticClass(), ItemQuantity);
	NiagaraComponent->SetActive(false);

	UE_LOG(LogTemp, Warning, TEXT("InventoryReference: %p"), InventoryReference);
	UE_LOG(LogTemp, Warning, TEXT("ItemReference: %p"), ItemReference);



	if(EndScreenClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("EndScreen set"));
		EndScreen = CreateWidget<UEndScreen>(GetWorld(), EndScreenClass);
		EndScreen->AddToViewport(999);
		EndScreen->SetVisibility(ESlateVisibility::Collapsed);
	}
	
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

	if(ItemDataTable && !RepairItemID.IsNone())
	{
		const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(RepairItemID, RepairItemID.ToString());

		RepairItemReference = NewObject<UBaseItem>(this, BaseClass);

		RepairItemReference->ID = ItemData->ID;
		RepairItemReference->ItemType = ItemData->ItemType;
		RepairItemReference->TextData = ItemData->TextData;
		RepairItemReference->NumericData = ItemData->NumericData;
		RepairItemReference->AssetData = ItemData->AssetData;

		InQuantity <= 0 ? RepairItemReference->SetQuantity(1) : RepairItemReference->SetQuantity(InQuantity);

		UE_LOG(LogTemp, Warning, TEXT("Init Stone Item"));
	}

	if(ItemDataTable && !RepairingDesiredItemID.IsNone())
	{
		const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(RepairingDesiredItemID, RepairingDesiredItemID.ToString());

		RepairingItemReference = NewObject<UBaseItem>(this, BaseClass);

		RepairingItemReference->ID = ItemData->ID;
		RepairingItemReference->ItemType = ItemData->ItemType;
		RepairingItemReference->TextData = ItemData->TextData;
		RepairingItemReference->NumericData = ItemData->NumericData;
		RepairingItemReference->AssetData = ItemData->AssetData;

		InQuantity <= 0 ? RepairingItemReference->SetQuantity(1) : RepairingItemReference->SetQuantity(InQuantity);

		UE_LOG(LogTemp, Warning, TEXT("Init Iron Item"));
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

	if(RepairWidgetClass)
	{
		RepairWidget = CreateWidget<URepairWidget>(GetWorld(), RepairWidgetClass);
		RepairWidget->AddToViewport(5);
		RepairWidget->WoodQuantity->SetText(FText::AsNumber(NumberOfWoodItemsDonated));
		RepairWidget->StoneQuantity->SetText(FText::AsNumber(NumberOfStoneItemsDonated));
		RepairWidget->IronQuantity->SetText(FText::AsNumber(NumberOfIronItemsDonated));
		RepairWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AObeliskActor::EndFocus()
{
	if(Mesh)
	{
		Mesh->SetRenderCustomDepth(false);
	}
	if(RepairWidget)
	{
		RepairWidget->AnimateClose();
	}
}

void AObeliskActor::BeginInteract()
{
	IInteractionInterface::BeginInteract();
	UE_LOG(LogTemp, Warning, TEXT("Test"));
	PlayRepairSound();
}

void AObeliskActor::EndInteract()
{
	IInteractionInterface::EndInteract();
	StopRepairSound();
	UE_LOG(LogTemp, Warning, TEXT("Test again"));
}

void AObeliskActor::Interact(ACupcakeCharacter* PlayerCharacter)
{
    InventoryReference = PlayerCharacter->GetInventory();

    // Process donations for each item type
    ProcessDonation(ItemReference, NumberOfWoodItemsDonated, RequiredWoodItems, TEXT("wood"), PlayerCharacter);
    ProcessDonation(RepairItemReference, NumberOfStoneItemsDonated, RequiredStoneItems, TEXT("stone"),PlayerCharacter);
    ProcessDonation(RepairingItemReference, NumberOfIronItemsDonated, RequiredIronItems, TEXT("iron"),PlayerCharacter);

    // Notify inventory update
    InventoryReference->OnInventoryUpdated.Broadcast();
}

void AObeliskActor::ProcessDonation(UBaseItem* &ItemRef, int &ItemsDonated, int RequiredItems, const TCHAR* ItemType, ACupcakeCharacter* PlayerCharacter)
{
	if (ItemRef)
	{
		UBaseItem* ItemToDonate = InventoryReference->FindMatchingItem(ItemRef);
		if (ItemToDonate)
		{
			int QuantityToDonate = FMath::Min(ItemToDonate->Quantity, RequiredItems - ItemsDonated);
			if (QuantityToDonate > 0)
			{
				PlayerCharacter->RemoveItemFromInventory(ItemToDonate, QuantityToDonate);
				ItemsDonated += QuantityToDonate;
				UpdateRepairWidget(ItemType, ItemsDonated);
				CheckIfDonationReached();
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("The obelisk does not need more %s items."), ItemType);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("The obelisk does not want that."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemRef was somehow null"));
	}
}


void AObeliskActor::UpdateRepairWidget(const TCHAR* ItemType, int Quantity)
{
    if (FCString::Stricmp(ItemType, TEXT("wood")) == 0)
    {
        RepairWidget->IncreaseWoodQuantity(Quantity);
    }
    else if (FCString::Stricmp(ItemType, TEXT("stone")) == 0)
    {
        RepairWidget->IncreaseStoneQuantity(Quantity);
    }
    else if (FCString::Stricmp(ItemType, TEXT("iron")) == 0)
    {
        RepairWidget->IncreaseIronQuantity(Quantity);
    }
}


bool AObeliskActor::CheckIfDonationReached()
{
	// Check if all conditions for donation completion are met
	bool hasStoneDonation = NumberOfStoneItemsDonated >= RequiredStoneItems; // assuming RequiredStoneItems is defined
	bool hasIronDonation = NumberOfIronItemsDonated >= RequiredIronItems;   // assuming RequiredIronItems is defined
	bool hasWoodDonation = NumberOfWoodItemsDonated >= RequiredWoodItems;   // assuming RequiredWoodItems is defined

	// All conditions must be true
	if (hasStoneDonation && hasIronDonation && hasWoodDonation)
	{
		UE_LOG(LogTemp, Warning, TEXT("Donation goal reached"));
		NiagaraComponent->SetActive(true);
		OnDonationGoalReached.Broadcast();
		EndScreen->SetVisibility(ESlateVisibility::Visible);
		DonationGoalReached = true; // Directly setting the state
	}
	else
	{
		DonationGoalReached = false;
	}

	return DonationGoalReached;
}



