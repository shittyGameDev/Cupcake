// Copyright Epic Games, Inc. All Rights Reserved.

#include "CupcakeGameMode.h"
#include "Engine.h" // Include this header for logging
#include "MapUI.h"
#include "Blueprint/UserWidget.h"

void ACupcakeGameMode::BeginPlay()
{
	 Super::BeginPlay();

    // Create an instance of your Widget Blueprint
    UUserWidget* WidgetInstance = CreateWidget<UUserWidget>(GetWorld(), UMapUI::StaticClass());

    // Add the widget to the viewport
    if (WidgetInstance)
    {
        WidgetInstance->AddToViewport();
    }
}


void ACupcakeGameMode::AddTriggerBox(bool bBoolValue, const FName& BoxName)
{
	// Add the bool value and trigger box name to the arrays
	TriggerBoxBoolArray.Add(bBoolValue);
	TriggerBoxNames.Add(BoxName);
}

bool ACupcakeGameMode::GetTriggerBoxBool(const FName& BoxName) const
{
	// Find the index of the trigger box name
	int32 Index = TriggerBoxNames.IndexOfByKey(BoxName);
	if (Index != INDEX_NONE)
	{
		// Return the bool value corresponding to the trigger box name
		return TriggerBoxBoolArray[Index];
	}
	return false;
}

void ACupcakeGameMode::LogTriggerBoxes() const
{
	UE_LOG(LogTemp, Warning, TEXT("Logging Trigger Boxes:"));
	
	for (int32 Index = 0; Index < TriggerBoxNames.Num(); ++Index)
	{
		UE_LOG(LogTemp, Warning, TEXT("Trigger Box Name: %s, Bool Value: %s"), *TriggerBoxNames[Index].ToString(), TriggerBoxBoolArray[Index] ? TEXT("true") : TEXT("false"));
	}
}

