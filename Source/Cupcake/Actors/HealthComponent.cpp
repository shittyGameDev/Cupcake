// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

#include "Health.h"
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

	Health = MaxHealth;
	
}

// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// ...
}

bool UHealthComponent::RegenerateHealth(float HealthAmount)
{
	if (MaxHealth == Health) return false;
	Health += HealthAmount;
	Health = FMath::Clamp(Health, 0.f, MaxHealth);
	return true;
}

void UHealthComponent::DoDamage(float DamageAmount)
{
	Health -= DamageAmount;
	Health = FMath::Clamp(Health, 0.f, MaxHealth);
	
	UE_LOG(LogTemp, Warning, TEXT("New Health: %f"), Health);

	if (Health <= 0)
	{
		AActor* Actor = Cast<AActor>(GetOwner());
		IHealth::Execute_OnDeath(Actor);
	}
}
