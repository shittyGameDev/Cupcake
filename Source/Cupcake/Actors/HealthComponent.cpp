// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

#include "DamagableInterface.h"
#include "Cupcake/PlayerSystem/CupcakeCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UHealthComponent::RegenerateHealth(float HealthAmount)
{
	if (MaxHealth == Health) return;
	Health += HealthAmount;
	Health = FMath::Clamp(Health, 0.f, MaxHealth);
}

void UHealthComponent::DoDamage(float DamageAmount)
{
	AActor* Actor = Cast<AActor>(GetOwner());
	
	if (Actor && Actor->GetClass()->ImplementsInterface(UDamagableInterface::StaticClass()))
	{
		IDamagableInterface::Execute_OnDamage(Actor);
	}
	
	Health -= DamageAmount;
	Health = FMath::Clamp(Health, 0.f, MaxHealth);
	
	UE_LOG(LogTemp, Warning, TEXT("New Health: %f"), Health);

	if (Health <= 0)
	{
		IDamagableInterface::Execute_OnDeath(Actor);
	}
}
