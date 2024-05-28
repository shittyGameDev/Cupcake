// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CupcakeGameMode.generated.h"

class UNewInventoryComponent;
class ACupcakeCharacter;

UCLASS()
class CUPCAKE_API ACupcakeGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:

public:
	virtual void BeginPlay() override;

	ACupcakeCharacter* PlayerCharacter;
	UNewInventoryComponent* PlayerInventory;

	TArray<AActor*> DamageableActors;
	
	UFUNCTION(BlueprintCallable)
	void SaveGame();

	UFUNCTION(BlueprintCallable)
	void LoadGame();
};
