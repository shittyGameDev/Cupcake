// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cupcake/Items/Data/ItemDataStructs.h"
#include "GameFramework/SaveGame.h"
#include "SavePlayerProgress.generated.h"


USTRUCT(BlueprintType)
struct FItemSaveData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	FName ID;

	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	int32 Quantity;

	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	EItemType ItemType;

	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	FItemTextData TextData;

	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	FItemNumericData NumericData;

	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	FItemAssetData AssetData;
};

UCLASS()
class CUPCAKE_API USavePlayerProgress : public USaveGame
{
	GENERATED_BODY()
public:
	// Player position
	UPROPERTY(VisibleAnywhere, Category ="SaveData")
	FVector PlayerPosition;

	//Player Rotation
	UPROPERTY(VisibleAnywhere, Category="SaveData")
	FRotator PlayerRotation;

	// Inventory contents
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	TArray<FItemSaveData> InventoryItems;
};
