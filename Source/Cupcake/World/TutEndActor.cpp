// Fill out your copyright notice in the Description page of Project Settings.


#include "TutEndActor.h"

// Sets default values
ATutEndActor::ATutEndActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");

	SetRootComponent(Mesh);

}

// Called when the game starts or when spawned
void ATutEndActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATutEndActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATutEndActor::BeginFocus()
{
	if(Mesh)
	{
		Mesh->SetRenderCustomDepth(true);
	}
}

void ATutEndActor::EndFocus()
{
	IInteractionInterface::EndFocus();
}

void ATutEndActor::BeginInteract()
{
	IInteractionInterface::BeginInteract();
}

void ATutEndActor::EndInteract()
{
	IInteractionInterface::EndInteract();
}

void ATutEndActor::Interact(ACupcakeCharacter* PlayerCharacter)
{
	IInteractionInterface::Interact(PlayerCharacter);
	ChangeLevel();
}

