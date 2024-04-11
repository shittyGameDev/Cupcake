// Fill out your copyright notice in the Description page of Project Settings.
// MapTriggerBoxSpecificActor.cpp

#include "MapTriggerBoxSpecificActor.h"

#include "MapUI.h"
#include "GameFramework/Character.h" // Include this header for character class

void AMapTriggerBoxSpecificActor::BeginPlay()
{
	Super::BeginPlay();

	// Bind overlap events
	OnActorBeginOverlap.AddDynamic(this, &AMapTriggerBoxSpecificActor::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &AMapTriggerBoxSpecificActor::OnOverlapEnd);

	// Initialize overlap status and player overlap flag
	bIsOverlapping = false;
	bPlayerOverlapped = false;
	
}

void AMapTriggerBoxSpecificActor::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{
	// Check if the overlapping actor is the player and the flag is not already set
	ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor);
	if (PlayerCharacter != nullptr && !bPlayerOverlapped)
	{
		// Set the bool to true when the player overlaps
		bIsOverlapping = true;


		// Log the player's name that is overlapping
		FString PlayerName = PlayerCharacter->GetName();
		UE_LOG(LogTemp, Warning, TEXT("%s is overlapping"), *PlayerName);

		// Access the UMapUI instance
		UMapUI* MapUI = Cast<UMapUI>(this);
		if (MapUI)
		{
			// Call the function to show images with the same trigger box name
			MapUI->ShowImagesOnTrigger(TriggerBoxName);
		}
	}

	ACupcakeGameMode* GameMode = GetWorld()->GetAuthGameMode<ACupcakeGameMode>();
	if (GameMode && !bPlayerOverlapped)
	{
		GameMode->AddTriggerBox(bIsOverlapping, TriggerBoxName); // Add triggerboxname and bool to array
		GameMode->LogTriggerBoxes(); // Call the logging function
	}
	bPlayerOverlapped = true; // Set the flag to true to prevent further setting
}

void AMapTriggerBoxSpecificActor::OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor)
{
	// Check if the overlapping actor is the player
	ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor);
	if (PlayerCharacter != nullptr)
	{
		// Set the bool to false when the player ends the overlap
		bIsOverlapping = false;
	}
}