// Fill out your copyright notice in the Description page of Project Settings.


#include "ForageableActor.h"

#include "Pickup.h"
#include "Cupcake/Items/BaseItem.h"
#include "Cupcake/Items/Data/ItemDataStructs.h"

// Sets default values
AForageableActor::AForageableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");

	SetRootComponent(Mesh);

}

// Called when the game starts or when spawned
void AForageableActor::BeginPlay()
{
	Super::BeginPlay();

	InteractableData = InstanceInteractableData;

	InitializeForageItem(UBaseItem::StaticClass(), ItemQuantity);
	
}

void AForageableActor::InitializeForageItem(const TSubclassOf<UBaseItem> BaseClass, const int32 InQuantity)
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
void AForageableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AForageableActor::BeginFocus()
{
	if(Mesh)
	{
		Mesh->SetRenderCustomDepth(true);
	}
}

void AForageableActor::EndFocus()
{
	if(Mesh)
	{
		Mesh->SetRenderCustomDepth(false);
	}
}

void AForageableActor::Interact(ACupcakeCharacter* PlayerCharacter)
{
	UE_LOG(LogTemp, Warning, TEXT("Interacting with forageable object"));

	if(!IsPendingKillPending())
	{
		if(ItemReference)
		{
			FVector SpawnLocation = GetActorLocation() + FMath::VRand() * FMath::RandRange(200.0f, 350.0f);
			FRotator SpawnRotation = FRotator(0.0f, FMath::RandRange(0.0f, 360.0f), 0.0f);

			APickup* Pickup = GetWorld()->SpawnActor<APickup>(APickup::StaticClass(), SpawnLocation, SpawnRotation);

			Pickup->InitializeDrop(ItemReference, ItemReference->Quantity);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Puckip internal item reference was somehow null!"));
		}
	}
}

