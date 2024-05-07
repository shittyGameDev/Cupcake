// Fill out your copyright notice in the Description page of Project Settings.


#include "Tree.h"

#include "Components/CapsuleComponent.h"
#include "Cupcake/Actors/AttributeComponent.h"

ATree::ATree() : IDamageableInterface(Attributes)
{
	PrimaryActorTick.bCanEverTick = false;

	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));

	Collider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collider"));
	SetRootComponent(Collider);
}

float ATree::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	return IDamageableInterface::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ATree::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATree::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

