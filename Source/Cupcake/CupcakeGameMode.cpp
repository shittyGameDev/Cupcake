// Copyright Epic Games, Inc. All Rights Reserved.

#include "CupcakeGameMode.h"

#include "Actors/AttributeComponent.h"
#include "Items/BaseItem.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerSystem/NewInventoryComponent.h"
#include "PlayerSystem/SavePlayerProgress.h"
#include "World/WorldSystem/DayCycleManager.h"
#include "Actors/DamageableInterface.h"
#include "UI/VolumeSave.h"
#include "World/WorldItems/ObeliskActor.h"

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
		SaveGameInstance->PlayerHealth = PlayerCharacter->Attributes->GetHealth();
		for (UBaseItem* Item : PlayerInventory->GetInventoryContents())
		{
			SaveGameInstance->InventoryItems.Add(ConvertToSaveData(Item));
		}
	}

	AObeliskActor* Boat = Cast<AObeliskActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AObeliskActor::StaticClass()));
	if(Boat)
	{
		SaveGameInstance->WoodDonated = Boat->NumberOfWoodItemsDonated;
		SaveGameInstance->SailDonated = Boat->NumberOfIronItemsDonated;
		SaveGameInstance->RudderDonated = Boat->NumberOfStoneItemsDonated;
	}

	// Get current Day (William)
	ADayCycleManager* DayCycleManager = Cast<ADayCycleManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ADayCycleManager::StaticClass()));
	if (DayCycleManager)
	{
		SaveGameInstance->DayNumber = DayCycleManager->GetCurrentDayNumber();
		UE_LOG(LogTemp, Warning, TEXT("Day cycle number: %d"), DayCycleManager->GetCurrentDayNumber());
	}

	// Save Active damageable actors in scene
	TArray<AActor*> LivingEnemies;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UDamageableInterface::StaticClass(), LivingEnemies);

	for (auto LivingEnemy : LivingEnemies)
	{
		SaveGameInstance->LivingEnemies.Add(LivingEnemy->GetName());
	}

	// Save Active collectables in scene
	TArray<AActor*> Collectables;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("KeyItem"), Collectables);

	for (auto Collectable : Collectables)
	{
		SaveGameInstance->Collectables.Add(Collectable->GetName());
	}
	
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
			PlayerCharacter->Attributes->SetHealth(LoadGameInstance->PlayerHealth);
			// Clear the inventory first
			PlayerInventory->ClearInventory();

			// Load each item from the save data
			for (const FItemSaveData& SaveData : LoadGameInstance->InventoryItems)
			{
				UBaseItem* NewItem = ConvertToBaseItem(this, SaveData);
				PlayerInventory->HandleLoadItem(NewItem);
			}
		}

		// Load day (William)
		/*ADayCycleManager* DayCycleManager = Cast<ADayCycleManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ADayCycleManager::StaticClass()));
		if (DayCycleManager)
		{
			for (int i = 0; i < LoadGameInstance->DayNumber - 1; i++)
			{
				// Shift day (default 1000 widget zindex)
				if (i != LoadGameInstance->DayNumber - 1)
				{
					DayCycleManager->ShiftDay();
				}
				// Shift day with higher zindex to out infront of others
				else
				{
					DayCycleManager->ShiftDay(1001);
				}
				
			}
			UE_LOG(LogTemp, Warning, TEXT("Target Day cycle: %d"), LoadGameInstance->DayNumber);
			UE_LOG(LogTemp, Warning, TEXT("Curent day: %d"), DayCycleManager->GetCurrentDayNumber());
		}*/

		// Remove enemies and collectables not in lists
		// Get enemies in scene
		TArray<AActor*> LivingEnemies;
		UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UDamageableInterface::StaticClass(), LivingEnemies);

		AActor* Player = nullptr;
		if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
		{
			Player = PlayerController->GetPawn();
		}
		
		for (auto LivingEnemy : LivingEnemies)
		{
			// Dont destroy player actor
			if (LivingEnemy != Player)
			{
				// Destroy if actor name is not in list
				if (!LoadGameInstance->LivingEnemies.Contains(LivingEnemy->GetName()))
				{
					LivingEnemy->Destroy();
				}
			}
		}

		// Save Active collectables in scene
		TArray<AActor*> Collectables;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("KeyItem"), Collectables);

		for (AActor* Collectable : Collectables)
		{
			// Destroy keyitem if not in saved list
			if (!LoadGameInstance->Collectables.Contains(Collectable->GetName()))
			{
				if (Collectable->ActorHasTag(TEXT("Map")))
				{
					Collectable->SetActorLocation(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation());
				}
				else
				{
					Collectable->Destroy();
				}
			}
		}

		AObeliskActor* Boat = Cast<AObeliskActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AObeliskActor::StaticClass()));
		if (Boat)
		{
			Boat->NumberOfWoodItemsDonated = LoadGameInstance->WoodDonated;
			Boat->NumberOfIronItemsDonated = LoadGameInstance->SailDonated;
			Boat->NumberOfStoneItemsDonated = LoadGameInstance->RudderDonated;
		}
		
	}
	UE_LOG(LogTemp, Warning, TEXT("Loaded Save"));
}

// Audio
void ACupcakeGameMode::SaveAudioSettings(float Master, float SFX, float Music)
{
	UVolumeSave* SaveVolumeInstance = Cast<UVolumeSave>(UGameplayStatics::CreateSaveGameObject(UVolumeSave::StaticClass()));
	
	SaveVolumeInstance->MasterVolume = Master;
	SaveVolumeInstance->MusicVolume = Music;
	SaveVolumeInstance->SFXVolume = SFX;

	UE_LOG(LogTemp, Warning, TEXT("Saved Aduio"));
	
	UGameplayStatics::SaveGameToSlot(SaveVolumeInstance, TEXT("AudioSettings"), 0);
}

float ACupcakeGameMode::GetMasterVolume()
{
	UVolumeSave* LoadAudioInstance = Cast<UVolumeSave>(UGameplayStatics::LoadGameFromSlot(TEXT("AudioSettings"), 0));
	if (LoadAudioInstance == nullptr)
	{
		SaveAudioSettings(1.f, 1.f, 1.f);
		return 1.f;
	}
	return LoadAudioInstance->MasterVolume;
}

float ACupcakeGameMode::GetSFXVolume()
{
	UVolumeSave* LoadAudioInstance = Cast<UVolumeSave>(UGameplayStatics::LoadGameFromSlot(TEXT("AudioSettings"), 0));
	if (LoadAudioInstance == nullptr)
	{
		SaveAudioSettings(1.f, 1.f, 1.f);
		return 1.f;
	}
	return LoadAudioInstance->SFXVolume;
}

float ACupcakeGameMode::GetMusicVolume()
{
	UVolumeSave* LoadAudioInstance = Cast<UVolumeSave>(UGameplayStatics::LoadGameFromSlot(TEXT("AudioSettings"), 0));
	if (LoadAudioInstance == nullptr)
	{
		SaveAudioSettings(1.f, 1.f, 1.f);
		return 1.f;
	}
	return LoadAudioInstance->MusicVolume;
}