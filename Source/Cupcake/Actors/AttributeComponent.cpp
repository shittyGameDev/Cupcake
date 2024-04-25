// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeComponent.h"

UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAttributeComponent::ReceiveDamage(float Damage)
{
	Health -= Damage;
	Health = FMath::Clamp(Health, 0.f, MaxHealth);
	
	UE_LOG(LogTemp, Warning, TEXT("New Health: %f"), Health);

	if (Health <= 0)
	{
		//UDamagableComponent::Execute_OnDeath(Actor);
	}
}

