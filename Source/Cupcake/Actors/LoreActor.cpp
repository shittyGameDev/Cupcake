// Fill out your copyright notice in the Description page of Project Settings.


#include "LoreActor.h"

// Sets default values
ALoreActor::ALoreActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LoreMesh"));
	Mesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ALoreActor::BeginPlay()
{
	Super::BeginPlay();

	InteractableData = InstanceInteractableData;
	
}

// Called every frame
void ALoreActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALoreActor::BeginFocus()
{
	if(Mesh)
	{
		//Mesh->SetRenderCustomDepth(true);
		SetOutline();
	}
	IInteractionInterface::BeginFocus();
}

void ALoreActor::EndFocus()
{
	if(Mesh)
	{
		//Mesh->SetRenderCustomDepth(false);
		RemoveOutline();
	}
	IInteractionInterface::EndFocus();
}

