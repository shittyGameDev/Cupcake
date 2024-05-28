// Copyright Epic Games, Inc. All Rights Reserved.

#include "CupcakeGameMode.h"

#include "Items/BaseItem.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerSystem/NewInventoryComponent.h"
#include "PlayerSystem/SavePlayerProgress.h"
#include "World/WorldSystem/DayCycleManager.h"
#include "Actors/DamageableInterface.h"

void ACupcakeGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Set references
	PlayerCharacter = Cast<ACupcakeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	PlayerInventory = PlayerCharacter->GetInventory();
}


//SAVE SYSTEM
//------------------------------------------------------------------

FItemSaveData ConvertToSaveData(UBaseItem* Item)
{
	FItemSaveData SaveData;
	SaveData.ID = Item->ID;
	SaveData.Quantity = Item->Quantity;
	SaveData.ItemType = Item->ItemType;
	SaveData.TextData = Item->TextData;
	SaveData.NumericData = Item->NumericData;
	SaveData.AssetData = Item->AssetData;
	return SaveData;
}

UBaseItem* ConvertToBaseItem(UObject* Outer, const FItemSaveData& SaveData)
{
	UBaseItem* Item = NewObject<UBaseItem>(Outer);
	Item->ID = SaveData.ID;
	Item->Quantity = SaveData.Quantity;
	Item->ItemType = SaveData.ItemType;
	Item->TextData = SaveData.TextData;
	Item->NumericData = SaveData.NumericData;
	Item->AssetData = SaveData.AssetData;
	return Item;
}

void ACupcakeGameMode::SaveGame()
{
	// Set references
	PlayerCharacter = Cast<ACupcakeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	PlayerInventory = PlayerCharacter->GetInventory();
	
	USavePlayerProgress* SaveGameInstance = Cast<USavePlayerProgress>(UGameplayStatics::CreateSaveGameObject(USavePlayerProgress::StaticClass()));
	
	//Get Inventory and save it (Victor)
	//Can be called in BP
	if (PlayerInventory)
	{
		for (UBaseItem* Item : PlayerInventory->GetInventoryContents())
		{
			SaveGameInstance->InventoryItems.Add(ConvertToSaveData(Item));
		}
	}

	// Get current Day (William)
	ADayCycleManager* DayCycleManager = Cast<ADayCycleManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ADayCycleManager::StaticClass()));
	if (DayCycleManager)
	{
		SaveGameInstance->DayNumber = DayCycleManager->GetCurrentDayNumber();
		UE_LOG(LogTemp, Warning, TEXT("Day cycle number: %d"), DayCycleManager->GetCurrentDayNumber());
	}

	// Save Active actors in scene
	TArray<AActor*> LivingEnemies;
	LivingEnemies.Empty();
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UDamageableInterface::StaticClass(), LivingEnemies);
	
	
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("PlayerSaveSlot"), 0);
	UE_LOG(LogTemp, Warning, TEXT("Saving Game"));
}

void ACupcakeGameMode::LoadGame()
{
	// Set references
	PlayerCharacter = Cast<ACupcakeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	PlayerInventory = PlayerCharacter->GetInventory();
    	
	USavePlayerProgress* LoadGameInstance = Cast<USavePlayerProgress>(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerSaveSlot"), 0));

	// Set everything we saved in the PlayerSaveSlot. Can be called in BP
	if (LoadGameInstance)
	{
		// Load inventory
		if (PlayerInventory)
		{
			// Clear the inventory first
			PlayerInventory->ClearInventory();

			// Load each item from the save data
			for (const FItemSaveData& SaveData : LoadGameInstance->InventoryItems)
			{
				UBaseItem* NewItem = ConvertToBaseItem(this, SaveData);
				PlayerInventory->HandleLoadItem(NewItem);
			}
		}

		// Load day
		ADayCycleManager* DayCycleManager = Cast<ADayCycleManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ADayCycleManager::StaticClass()));
		if (DayCycleManager)
		{
			for (int i = 0; i < LoadGameInstance->DayNumber - 1; i++)
			{
				if (i != LoadGameInstance->DayNumber - 1)
				{
					DayCycleManager->ShiftDay();
				}
				else
				{
					DayCycleManager->ShiftDay(1001);
				}
				
			}
			UE_LOG(LogTemp, Warning, TEXT("Target Day cycle: %d"), LoadGameInstance->DayNumber);
			UE_LOG(LogTemp, Warning, TEXT("Curent day: %d"), DayCycleManager->GetCurrentDayNumber());
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Loaded Save"));
}