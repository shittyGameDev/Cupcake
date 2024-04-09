// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldEvent.h"

// Sets default values for this component's properties
UWorldEvent::UWorldEvent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// ...
}


void UWorldEvent::Run()
{
	OnRunTriggered();
}

// Called when the game starts
void UWorldEvent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UWorldEvent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

