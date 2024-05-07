// Fill out your copyright notice in the Description page of Project Settings.


#include "Tree.h"

#include "Cupcake/Actors/AttributeComponent.h"

ATree::ATree()
{
	PrimaryActorTick.bCanEverTick = false;

	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
}

void ATree::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATree::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

