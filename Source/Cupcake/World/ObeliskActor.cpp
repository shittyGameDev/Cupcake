// Fill out your copyright notice in the Description page of Project Settings.


#include "ObeliskActor.h"

#include "NiagaraComponent.h"
#include "Cupcake/Items/BaseItem.h"
#include "Cupcake/Items/Data/ItemDataStructs.h"
#include "Cupcake/UI/RepairWidget.h"
#include "Cupcake/PlayerSystem/NewInventoryComponent.h"

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

	if(RepairWidgetClass)
	{
		RepairWidget = CreateWidget<URepairWidget>(GetWorld(), RepairWidgetClass);
		RepairWidget->AddToViewport(5);
		RepairWidget->SetVisibility(ESlateVisibility::Collapsed);
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

	if(RepairWidget)
	{
		RepairWidget->SetVisibility(ESlateVisibility::Visible); //Tydligen har Collapsed en bättre påverkan på performance än Hidden
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
		RepairWidget->SetVisibility(ESlateVisibility::Collapsed);
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
		if(ItemToDonate)
		{
			UE_LOG(LogTemp, Warning, TEXT("Test"));
			NumberOfWoodItemsDonated = ItemToDonate->Quantity;
			PlayerCharacter->RemoveItemFromInventory(ItemToDonate, ItemReference->Quantity);
			CheckIfDonationReached();
			InventoryReference->OnInventoryUpdated.Broadcast();
			RepairWidget->IncreaseWoodQuantity(NumberOfWoodItemsDonated);
			return;
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
	if(RepairItemReference)
	{
		UBaseItem* ItemToDonate = InventoryReference->FindMatchingItem(RepairItemReference);
		if(ItemToDonate)
		{
			UE_LOG(LogTemp, Warning, TEXT("Test"));
			PlayerCharacter->RemoveItemFromInventory(ItemToDonate, RepairItemReference->Quantity);
			NumberOfStoneItemsDonated++;
			CheckIfDonationReached();
			InventoryReference->OnInventoryUpdated.Broadcast();
			RepairWidget->IncreaseStoneQuantity(NumberOfIronItemsDonated);
			return;
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
	if(RepairingItemReference)
	{
		UBaseItem* ItemToDonate = InventoryReference->FindMatchingItem(RepairingItemReference);
		if(ItemToDonate)
		{
			UE_LOG(LogTemp, Warning, TEXT("Test"));
			PlayerCharacter->RemoveItemFromInventory(ItemToDonate, RepairingItemReference->Quantity);
			NumberOfIronItemsDonated++;
			CheckIfDonationReached();
			InventoryReference->OnInventoryUpdated.Broadcast();
			RepairWidget->IncreaseIronQuantity(NumberOfIronItemsDonated);
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

bool AObeliskActor::CheckIfDonationReached()
{
	if(NumberOfWoodItemsDonated && NumberOfIronItemsDonated && NumberOfStoneItemsDonated == 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("Donation goal reached"));
		NiagaraComponent->SetActive(true);
		OnDonationGoalReached.Broadcast();
		return DonationGoalReached = true;
		//Call custom event, actor move.
	}
	return DonationGoalReached = false;
}


