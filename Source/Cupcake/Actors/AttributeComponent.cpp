// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeComponent.h"

#include "DamageableInterface.h"

UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = 100.f;
	MaxHealth = 100.f;
}

void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAttributeComponent::ReceiveDamage(float Damage)
{
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
	
	UE_LOG(LogTemp, Warning, TEXT("%s Health: %f"), *GetName(), Health);

	if (Health <= 0)
	{
		IDamageableInterface::Execute_OnDeath(GetOwner());
	}
}

