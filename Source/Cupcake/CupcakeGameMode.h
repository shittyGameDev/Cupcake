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

	UPROPERTY()
	ACupcakeCharacter* PlayerCharacter;
	UPROPERTY()
	UNewInventoryComponent* PlayerInventory;
	
	UFUNCTION(BlueprintCallable)
	void SaveGame();

	UFUNCTION(BlueprintCallable)
	void LoadGame();

	// Audio
	UFUNCTION(BlueprintCallable)
	void SaveAudioSettings(float Master, float SFX, float Music);

	UFUNCTION(BlueprintCallable)
	float GetMasterVolume();
	UFUNCTION(BlueprintCallable)
	float GetSFXVolume();
	UFUNCTION(BlueprintCallable)
	float GetMusicVolume();
};
