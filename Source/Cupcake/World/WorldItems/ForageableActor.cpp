// Fill out your copyright notice in the Description page of Project Settings.


#include "ForageableActor.h"
#include "NiagaraComponent.h"
#include "Pickup.h"
#include "Cupcake/Items/BaseItem.h"
#include "Cupcake/Items/Data/ItemDataStructs.h"
#include "Cupcake/PlayerSystem/NewInventoryComponent.h"

// Sets default values
AForageableActor::AForageableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");

	SetRootComponent(Mesh);

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("NiagaraComponent");
	NiagaraComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AForageableActor::BeginPlay()
{
	Super::BeginPlay();

	InteractableData = InstanceInteractableData;

	bIsForageable = true;

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

	if (!IsPendingKillPending() && bIsForageable)
	{
		if (ItemReference)
		{
			InventoryComponent = PlayerCharacter->GetInventory();
			InventoryComponent->HandleAddItem(ItemReference);
			PlayForageSound();
			StartForagingTimer();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Puckip internal item reference was somehow null!"));
		}
	}
}

void AForageableActor::HandleTimerFinished()
{
	// Set bIsForageable to true when the timer finishes
	bIsForageable = true;
	NiagaraComponent->SetVisibility(true);
}

void AForageableActor::StartForagingTimer()
{
	// Set the timer for 5 seconds duration
	bIsForageable = false;
	NiagaraComponent->SetVisibility(false);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AForageableActor::HandleTimerFinished, 5.0f, false);
}

FTransform AForageableActor::CalculateSpawnPoint() const
{
	const float Angle = FMath::RandRange(0.0f, 360.0f);
	const float Radius = FMath::RandRange(150.0f, 300.0f);
	const FVector Direction = FVector(FMath::Cos(FMath::DegreesToRadians(Angle)), FMath::Sin(FMath::DegreesToRadians(Angle)), 0.0f);
	const FVector SpawnLocation = GetActorLocation() + Direction * Radius;
	const FTransform SpawnTransform(FRotator(0.0f, Angle, 0.0f), SpawnLocation);

	return SpawnTransform;
}





