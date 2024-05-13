// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialWall.h"

#include "Components/BoxComponent.h"
#include "Cupcake/PlayerSystem/CupcakeCharacter.h"
#include "Cupcake/PlayerSystem/NewInventoryComponent.h"

// Sets default values
ATutorialWall::ATutorialWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	BoxComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ATutorialWall::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATutorialWall::TurnOffTutWall() const
{
	BoxComponent->DestroyComponent();
}

// Called every frame
void ATutorialWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(PlayerCharacter && !bIsTurnedOff)
	{
		InventoryComponent = PlayerCharacter->GetInventory();
		if(InventoryComponent->HasItemByID("test_001"))
		{
			TurnOffTutWall();
			bIsTurnedOff = true;
			UE_LOG(LogTemp, Warning, TEXT("I turned off"));
		}
	}
}

