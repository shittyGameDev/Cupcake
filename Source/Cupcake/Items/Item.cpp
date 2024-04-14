// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

#include "IDetailTreeNode.h"
#include "Cupcake/PlayerSystem/CupcakeCharacter.h"
#include "Kismet/GameplayStatics.h"

class UInventoryComponent;

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	RootComponent = ItemMeshComponent;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisioComponent"));
	CollisionComponent->InitSphereRadius(50.0f);
	CollisionComponent->SetCollisionProfileName(TEXT("Trigger"));
	CollisionComponent->SetupAttachment(RootComponent);

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnOverlapBegin);
	
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	ItemName = ItemName.ToLower();
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                           int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		
		if(auto const Player = Cast<ACupcakeCharacter>(OtherActor))
		{
			if(UInventoryComponent* Inventory = Player->FindComponentByClass<UInventoryComponent>())
			{
				if(this != nullptr)
				{
					Inventory->AddItem(this);

					SetActorEnableCollision(false);
					SetActorHiddenInGame(true);
				}
			}
		}
	}
	
}

void AItem::Interact_Implementation()
{
	//IInteractable::Interact_Implementation();
	if (ACupcakeCharacter* Player = Cast<ACupcakeCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
	{
		if(UInventoryComponent* Inventory = Player->FindComponentByClass<UInventoryComponent>())
		{
			Inventory->AddItem(this);
		}
	}
}

